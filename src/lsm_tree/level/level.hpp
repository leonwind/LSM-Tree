#ifndef LEVEL_H
#define LEVEL_H

#include "../../bloom_filter/bloom.hpp"
#include "../../red_black_tree/red_black.hpp"
#include "../../utils/types.hpp"
#include <string>
#include <queue>
#include <ios>
#include <filesystem>

class level {

    public:
        level(const std::string &path, long bloom_size, red_black_tree &memtable);

        level(const std::string &path, level &sst_a, level &sst_b, long bloom_size);

        level(const std::string &path, long bloom_size);

        ~level() = default;

        std::optional<std::string> search(const std::string& target) const;

        static std::string create_filename_based_on_level(uint16_t id, uint16_t level_order);

        static std::pair<uint16_t, std::list<std::pair<uint32_t, std::vector<level>>>>
            collect_levels(const std::string& path, uint64_t memtable_size);

        void delete_segment_file();

        static void delete_all_segments(const std::string& path);

        std::string get_name() const;

    protected:
        std::queue<kv_pair> get_kv_pairs() const;

    private:
        static const uint64_t SPARSITY_FACTOR{1000};

        std::string path;
        bloom_filter bloom;
        red_black_tree index;

        void create_sst_from_memtable(red_black_tree& memtable);

        void merge_sst_values(const level& sst_a, const level& sst_b);

        void repopulate_bloom_and_index();

        static void print_queue(std::queue<kv_pair> queue1);
};

#endif // LEVEL_H
