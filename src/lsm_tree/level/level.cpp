#include "level.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>


level::level(const std::string &path, long bloom_size, red_black_tree &memtable)
        : bloom(bloom_size), index() {
    this->path = path;

    create_sst_from_memtable(memtable);
}

level::level(const std::string &path, level &sst_a, level &sst_b, long bloom_size)
        : bloom(bloom_size), index() {
    this->path = path;
    merge_sst_values(sst_a, sst_b);
}

std::string level::get_name() {
    return path;
}

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

std::optional<std::string> level::search(const std::string &target) {
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
    std::string key, value;

    while (std::getline(sst, line)) {
        size_t seperator_pos = line.find(SEPERATOR);
        key = line.substr(0, seperator_pos);
        value = line.substr(seperator_pos + 1, line.size());

        kv_pairs.push({key, value});
    }

    return kv_pairs;
}

/**
 * Merge two Sorted String Tables and write them sorted to the disk.
 */
void level::merge_sst_values(const level& sst_a, const level& sst_b) {
    std::cout << "Start merging " << sst_a.path << " with " << sst_b.path << std::endl;

    auto queue_a = sst_a.get_kv_pairs();
    std::cout << "Queue A:" << std::endl;
    print_queue(queue_a);
    auto queue_b = sst_b.get_kv_pairs();
    std::cout << "Queue B:" << std::endl;
    print_queue(queue_b);

    std::ofstream sst;
    sst.open(path);

    kv_pair next_pair;
    uint64_t sparsity_i{0};

    std::unordered_set<std::string> deleted;

    while ((not queue_a.empty()) or (not queue_b.empty())) {

        if (queue_b.empty() or queue_a.front() < queue_b.front()) {
            next_pair = kv_pair{queue_a.front()};
            queue_a.pop();
        } else {
            next_pair = kv_pair{queue_b.front()};
            queue_b.pop();
        }

        if (deleted.contains(next_pair.key)) {
            continue;
        }

        if (next_pair.val == TOMBSTONE) {
            deleted.insert(next_pair.key);
        }

        std::cout << "NEXT PAIR: " << next_pair.to_log_entry() << std::endl;
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
        std::cout << curr.to_log_entry() << std::endl;
        q.pop();
    }
}