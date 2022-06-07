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
level::level(const std::string &path, level &sst_a, level &sst_b, long bloom_size)
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

/**
 * Get all nodes from memtable in-order and write them to the disk.
 * Populate the bloom filter and insert every SPARSITY_INDEX'th node inside
 * the sparse index.
 */
void level::create_sst_from_memtable(red_black_tree &memtable) {
    std::ofstream sst;
    sst.open(path);

    uint64_t sparsity_i{0};

    std::vector<rb_entry> rb_nodes = memtable.get_and_delete_all_nodes();
    for (rb_entry& node : rb_nodes) {
        kv_pair pair = {node.key, std::any_cast<std::string>(node.val.value())};
        bloom.set(pair.key);

        std::string log_entry = pair.to_log_entry();
        sst << log_entry << std::flush;

        if (sparsity_i++ == SPARSITY_FACTOR) {
            index.insert(pair);
            sparsity_i = 0;
        }
    }
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
        start_pos = std::any_cast<long>(opt_floor_node.value());
    }

    std::ifstream sst(path, std::ios_base::in);
    if (sst.is_open()) {
        sst.seekg(start_pos, std::ios_base::beg);
        std::string line, key;

        while (std::getline(sst, line)) {
            size_t seperator_pos = line.find(SEPERATOR);
            key = line.substr(0, seperator_pos);

            if (target < key) {
                // Since SST is sorted, we can stop when key is greater than
                // target.
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
 * Iterate over both files and merge them inside a queue
 * TODO: Merge without storing them in-memory and use iterator over both SSTs.
 */
std::queue<kv_pair> level::get_kv_pairs() const {
    std::queue<kv_pair> kv_pairs{};
    std::ifstream sst(path, std::ios_base::in);

    if (not sst.is_open()) {
        return {};
    }

    std::string line;

    while (std::getline(sst, line)) {
        size_t seperator_pos = line.find(SEPERATOR);
        std::string key = line.substr(0, seperator_pos);
        std::string value = line.substr(seperator_pos + 1, line.size() - seperator_pos);

        kv_pairs.push({key, value});
    }

    return kv_pairs;
}

/**
 * Merge two Sorted String Tables and write them sorted to the disk.
 */
void level::merge_sst_values(const level& sst_a, const level& sst_b) {
    std::cout << "Start merging " << sst_a.path << " with "
        << sst_b.path << " to file " << path << std::endl;

    auto queue_a = sst_a.get_kv_pairs();
    auto queue_b = sst_b.get_kv_pairs();

    std::ofstream sst;
    sst.open(path);

    uint64_t sparsity_i{0};
    std::string last_key;

    while ((not queue_a.empty()) or (not queue_b.empty())) {

        kv_pair next_pair;
        if (not queue_a.empty() and (queue_b.empty() or queue_a.front() < queue_b.front())) {
            next_pair = kv_pair{queue_a.front()};
            queue_a.pop();
        } else {
            next_pair = kv_pair{queue_b.front()};
            queue_b.pop();
        }

        // If we had this key earlier, we take the latest value which is the earlier
        // kv pair.
        if (last_key == next_pair.key) {
            continue;
        } else {
            last_key = next_pair.key;
        }

        std::cout << "NEXT PAIR: " << next_pair.to_log_entry();
        bloom.set(next_pair.key);

        if (sparsity_i++ == SPARSITY_FACTOR) {
            std::cout << "Insert into Index" << std::endl;
            index.insert(next_pair);
            sparsity_i = 0;
        }

        sst << next_pair.to_log_entry() << std::flush;
    }

    std::cout << "Finished merging" << std::endl;
}

/**
 * Iterate over the SST and populate the bloom filter and the Sparse Index.
 */
void level::repopulate_bloom_and_index() {
    auto kv_pairs = get_kv_pairs();
    uint64_t sparsity_i{0};

    while(not kv_pairs.empty()) {
        kv_pair curr_pair = kv_pairs.front();
        kv_pairs.pop();

        bloom.set(curr_pair.key);
        if (sparsity_i++ == SPARSITY_FACTOR) {
            index.insert(curr_pair);
            sparsity_i = 0;
        }
    }
}

/**
 * Collect all stored segment files in @param path and return a list of all segments
 * with its respectful level hierarchy.
 * Retrieve the level of its segment based on the filename.
 * Return the segment and the largest ID.
 */
std::pair<uint16_t, std::list<std::pair<uint32_t, std::vector<level>>>>
    level::collect_levels(const std::string &path, uint64_t memtable_size) {

    std::map<uint32_t, std::vector<level>> cache;

    uint16_t largest_id{0};

    for (const auto& segment_file : std::filesystem::directory_iterator(path)) {
        std::string segment_path = segment_file.path().string();
        std::string filename = segment_path.substr(path.find_last_of("/\\") + 1);

        // id and level_order is always 00001_00001 (id_level)
        largest_id = std::max((uint16_t) std::stoul(filename.substr(0, 5)), largest_id);
        uint16_t level_order = std::stoi(filename.substr(6, 5));

        level sst = level(segment_path, memtable_size * (level_order + 1));

        if (cache.contains(level_order)) {
            cache[level_order].push_back(sst);
        } else {
            cache[level_order] = {sst};
        }
    }

    std::list<std::pair<uint32_t, std::vector<level>>> segments;
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

void level::print_queue(std::queue<kv_pair> q) {
    while (not q.empty()) {
        auto curr = q.front();
        q.pop();
        std::cout << curr.to_log_entry();
    }
}
