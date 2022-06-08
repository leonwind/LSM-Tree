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

        level(const std::string &path, level* sst_a, level* sst_b, long bloom_size);

        level(const std::string &path, long bloom_size);

        ~level();

        std::optional<std::string> search(const std::string& target) const;

        static std::string
            create_filename_based_on_level(uint16_t id, uint16_t level_order);

        static std::pair<uint16_t, uint16_t>
            extract_id_level_from_path(const std::string& path);

        static std::pair<uint16_t, std::list<std::pair<uint32_t, std::vector<level*>>>>
            collect_levels(const std::string& path, uint64_t memtable_size);

        static void delete_all_segments(const std::string& path);

        std::string get_name() const;

    private:
        static const uint64_t SPARSITY_FACTOR{100};

        std::string path;
        bloom_filter bloom;
        red_black_tree index;

        void create_sst_from_memtable(red_black_tree& memtable);

        void merge_sst_values(level* sst_a, level* sst_b);

        void repopulate_bloom_and_index();

        void delete_segment_file();
};

#endif // LEVEL_H
