#ifndef LEVEL_H
#define LEVEL_H

#include "../../bloom_filter/bloom.hpp"
#include "../../red_black_tree/red_black.hpp"
#include "../../utils/types.hpp"
#include <string>
#include <queue>
#include <ios>

class level {

    public:
        level(const std::string &path, size_t max_size, const bloom_filter &filter);

        ~level() = default;

        level merge(const level& other);

        std::optional<std::string> search(const std::string& key);

    private:
        std::string path;
        size_t max_size;
        size_t curr_size{0};
        bloom_filter filter;
        red_black_tree index;

        static std::queue<kv_pair> get_kv_pairs(const std::string& path);
        static std::pair<bloom_filter, std::queue<kv_pair>> merge_sst_values(
                std::queue<kv_pair>& queue_a, std::queue<kv_pair>& queue_b);
};

#endif // LEVEL_H
