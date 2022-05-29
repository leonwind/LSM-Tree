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
        level(const std::string &path, const bloom_filter &filter);

        level(const std::string &path, long bloom_size, red_black_tree &memtable);

        level(const std::string &path, level &sst_a, level &sst_b, long bloom_size);

        ~level() = default;

        std::optional<std::string> search(const std::string& key);

    private:
        static const uint64_t SPARSITY_FACTOR{1000};

        std::string path;
        bloom_filter bloom;
        red_black_tree index;

        void create_sst_from_memtable(red_black_tree& memtable);

        static std::queue<kv_pair> get_kv_pairs(const std::string& path);
        void merge_sst_values(const level& sst_a, const level& sst_b);
};

#endif // LEVEL_H
