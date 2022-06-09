#include "level.hpp"
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>

/**
 * Init a new SST based on the flushed Memtable.
 */
level::level(const std::string &path, long bloom_size, red_black_tree &memtable)
        : bloom(bloom_size), index() {
    this->path = path;
    create_sst_from_memtable(memtable);
}

/**
 * Merge two existing SSTs into a new one.
 */
level::level(const std::string &path, level* sst_a, level* sst_b, long bloom_size)
        : bloom(bloom_size), index() {
    this->path = path;
    merge_sst_values(sst_a, sst_b);
}

/**
 * Create a new SST based on its existing segment file.
 * Only needed for repopulating the segments into Memory after restarting the db.
 */
level::level(const std::string& path, long bloom_size)
        : bloom(bloom_size), index() {
    this->path = path;
    repopulate_bloom_and_index();
}

level::~level() {
    index.delete_tree();
    delete_segment_file();
}

std::string level::get_name() const {
    return path;
}

/*
 * Create a new filename based on the segment ID and the level of the segment, e.g.
 * 00001_00010 (id = 1, level = 10).
 */
std::string level::create_filename_based_on_level(uint16_t id, uint16_t level_order) {
    std::ostringstream ss;
    // Pad id and level_order to 00000 since uint16_t is max 65535.
    ss << std::setw(5) << std::setfill('0') << id << "_";
    ss << std::setw(5) << std::setfill('0') << level_order;
    return ss.str();
}

std::pair<uint16_t, uint16_t> level::extract_id_level_from_path(const std::string& path) {
    std::string filename = path.substr(path.find_last_of("/\\") + 1);

    uint16_t id = (uint16_t) std::stoul(filename.substr(0, 5));
    uint16_t level_order = std::stoi(filename.substr(6, 5));

    return {id, level_order};
}

/**
 * Get all nodes from memtable in-order and write them to the disk.
 * Populate the bloom filter and insert every SPARSITY_INDEX'th node inside
 * the sparse index.
 */
void level::create_sst_from_memtable(red_black_tree &memtable) {
    std::ofstream sst;
    sst.open(path);

    uint64_t sparsity_i{0};
    uint64_t pos{0};

    std::vector<rb_entry> rb_nodes = memtable.get_and_delete_all_nodes();
    for (rb_entry& node : rb_nodes) {
        kv_pair pair = {node.key, std::any_cast<std::string>(node.val.value())};
        bloom.set(pair.key);

        std::string log_entry = pair.to_log_entry();
        sst << log_entry;

        if (sparsity_i++ == SPARSITY_FACTOR) {
            index.insert(rb_entry{pair.key, pos});
            sparsity_i = 0;
        }

        pos += log_entry.size();
    }
    sst << std::flush;
}

/**
 * Search on the SST.
 * First check if the key exists in the bloom filter, if not we are certain that
 * the key does not exists.
 * Then, use the Sparse Index to get a close lower neighbour of the key, every
 * 1000th (SPARSITY_FACTOR) key should exists in the Sparse Index.
 * Iterate from the lower neighbour upwards until we hit a key > target and we
 * can stop.
 */
std::optional<std::string> level::search(const std::string &target) const {
    if (not bloom.is_set(target)) {
        return {};
    }

    std::optional<rb_entry> opt_floor_node = index.floor(target);
    long start_pos{0};

    if (opt_floor_node.has_value()) {
        start_pos = (long) std::any_cast<uint64_t>(opt_floor_node.value().val.value());
    }

    std::ifstream sst(path, std::ios_base::in);
    if (sst.is_open()) {
        sst.seekg(start_pos, std::ios_base::beg);
        std::string line, key;

        while (std::getline(sst, line)) {
            size_t seperator_pos = line.find(SEPERATOR);
            key = line.substr(0, seperator_pos);

            if (target < key) {
                // Since SST is sorted, we can stop when key is greater than target.
                return {};
            }

            if (target == key) {
                return {line.substr(seperator_pos + 1, line.size())};
            }
        }
    }

    return {};
}

/**
 * Merge two Sorted String Tables and write them sorted to the disk.
 * Remove duplicated or deleted objects.
 */
void level::merge_sst_values(level* sst_a, level* sst_b) {
    // SST_a is always the latest SST and dominates over SST_b.
    if (extract_id_level_from_path(sst_a->path).first < extract_id_level_from_path(sst_b->path).first) {
        std::swap(sst_a, sst_b);
    }

    std::ifstream sst_a_file(sst_a->path, std::ios_base::in);
    std::ifstream sst_b_file(sst_b->path, std::ios_base::in);

    std::ofstream merged_sst;
    merged_sst.open(path);

    std::string last_key;
    uint64_t sparsity_i{0};
    uint64_t pos{0};

    std::string line_a, line_b;
    kv_pair kv_pair_a, kv_pair_b;
    kv_pair min_kv_pair;

    bool sst_a_empty = false;
    bool sst_b_empty = false;

    while (true) {

        if (line_a.empty() and not sst_a_empty) {
            std::getline(sst_a_file, line_a);
            sst_a_empty = line_a.empty();

            kv_pair_a = kv_pair::split_log_entry(line_a);
        }

        if (line_b.empty() and not sst_b_empty) {
            std::getline(sst_b_file, line_b);
            sst_b_empty = line_b.empty();

            kv_pair_b = kv_pair::split_log_entry(line_b);
        }

        if (kv_pair_a.empty() and kv_pair_b.empty()) {
            break;
        }

        if (kv_pair_a.empty() and not kv_pair_b.empty()) {
            min_kv_pair = kv_pair_b;
            line_b.clear();
        } else if (not kv_pair_a.empty() and kv_pair_b.empty()) {
            min_kv_pair = kv_pair_a;
            line_a.clear();
        } else {
            if (kv_pair_a <= kv_pair_b) {
                min_kv_pair = kv_pair_a;
                line_a.clear();
            } else {
                min_kv_pair = kv_pair_b;
                line_b.clear();
            }
        }

        if (last_key == min_kv_pair.key) {
            continue;
        } else {
            last_key = min_kv_pair.key;
        }

        //std::cout << "NEXT PAIR: " << min_kv_pair.to_log_entry();
        bloom.set(min_kv_pair.key);

        if (sparsity_i++ == SPARSITY_FACTOR) {
            index.insert(rb_entry{min_kv_pair.key, pos});
            sparsity_i = 0;
        }

        std::string log_entry = min_kv_pair.to_log_entry();
        merged_sst << log_entry;

        pos += log_entry.size();
    }
    merged_sst << std::flush;
}

/**
 * Iterate over the SST and repopulate the bloom filter and the Sparse Index.
 */
void level::repopulate_bloom_and_index() {
    std::ifstream sst_file(path, std::ios_base::in);
    std::string line;

    uint64_t sparsity_i{0};
    uint64_t pos{0};

    while (std::getline(sst_file, line)) {
        kv_pair curr_pair = kv_pair::split_log_entry(line);
        bloom.set(curr_pair.key);

        if (sparsity_i++ == SPARSITY_FACTOR) {
            index.insert(rb_entry{curr_pair.key, pos});
            sparsity_i = 0;
        }

        pos += line.size();
    }
}

/**
 * Collect all stored segment files in @param path and return a list of all segments
 * with its respectful level hierarchy.
 * Retrieve the level of its segment based on the filename.
 * Return the segment and the largest ID.
 */
std::pair<uint16_t, std::list<std::pair<uint32_t, std::vector<level*>>>>
    level::collect_levels(const std::string &path, uint64_t memtable_size) {

    std::map<uint16_t, std::vector<level*>> cache{};

    uint16_t largest_id{0};

    for (const auto& segment_file : std::filesystem::directory_iterator(path)) {
        std::string segment_path = segment_file.path().string();
        auto id_level = extract_id_level_from_path(segment_path);

        largest_id = std::max(largest_id, id_level.first);
        uint16_t level_order = id_level.second;

        auto* sst = new level(segment_path, (long) memtable_size * (level_order + 1) * 2);

        if (cache.contains(level_order)) {
            cache[level_order].push_back(sst);
        } else {
            cache[level_order] = {sst};
        }
    }

    std::list<std::pair<uint32_t, std::vector<level*>>> segments;
    for (const auto& sst : cache) {
        segments.emplace_back(sst);
    }

    return {largest_id, segments};
}

void level::delete_segment_file() {
    std::filesystem::remove(path);
}

void level::delete_all_segments(const std::string &path) {
    for (const auto& entry : std::filesystem::directory_iterator(path))
        std::filesystem::remove_all(entry.path());
}
