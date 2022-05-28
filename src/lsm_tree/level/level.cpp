#include "level.hpp"
#include <fstream>

level::level(const std::string &path, size_t max_size, const bloom_filter &filter)
        : filter(filter) {
    this->path = path;
    this->max_size = max_size;
}

std::optional<std::string> level::search(const std::string &key) {
    std::ifstream sst(path, std::ios_base::in);
    if (not sst.is_open()) {
        return {};
    }

    size_t left{0};
    size_t right
}

level level::merge(const level& other){
    auto kv_pairs_a = get_kv_pairs(path);
    auto kv_pairs_b = get_kv_pairs(other.path);
    auto merged = merge_sst_values(kv_pairs_a, kv_pairs_b);

    return other;
}

std::queue<kv_pair> level::get_kv_pairs(const std::string& path) {
    std::queue<kv_pair> kv_pairs{};
    std::ifstream sst(path, std::ios_base::in);

    if (not sst.is_open()) {
        return {};
    }

    std::string line;
    std::string key, value;

    while (std::getline(sst, line)) {
        size_t seperator_pos = line.find(',');
        key = line.substr(0, seperator_pos);
        value = line.substr(seperator_pos + 1, line.size());

        kv_pairs.push({key, value});
    }

    return kv_pairs;
}

std::pair<bloom_filter, std::queue<kv_pair>> level::merge_sst_values(
        std::queue<kv_pair> &queue_a, std::queue<kv_pair> &queue_b) {

    bloom_filter bloom(1000);
    std::queue<kv_pair> merged{};

    kv_pair next_pair{};
    while (not queue_a.empty() and not queue_b.empty()) {
        if (queue_a.empty() or queue_b.front() < queue_a.front()) {
            next_pair = queue_b.front();
            queue_b.pop();
        } else {
            next_pair = queue_b.front();
            queue_a.pop();
        }

        merged.push(next_pair);
        bloom.set(next_pair.key);
    }

    return std::make_pair(bloom, merged);
}
