#ifndef LSM_TREE_H
#define LSM_TREE_H

#include "../utils/types.hpp"
#include "../bloom_filter/bloom.hpp"
#include "../red_black_tree/red_black.hpp"
#include "../wal/wal.hpp"
#include "level/level.hpp"
#include <string>
#include <list>
#include <vector>
#include <optional>

#define WAL_PATH "../src/.internal_storage/wal.log"
#define SEGMENT_BASE "../src/.internal_storage/segments/"

class lsm_tree {

    public:
        lsm_tree();

        ~lsm_tree();
        
        void put(const std::string& key, const std::string& value);

        std::string get(const std::string& key);

        void remove(const std::string& key);

        void drop_table();

    private:
        static const uint64_t MEMTABLE_SIZE{100000};

        red_black_tree memtable;
        std::list<std::pair<uint32_t, std::vector<level*>>> segments;
        write_ahead_log wal;
        uint16_t segment_i{0};

        void flush_memtable_to_disk();

        void restore_db();

        void restore_memtable();

        void restore_segments();

        void compact();

        std::optional<std::string> search_all_segments(const std::string& target);

        std::string get_new_segment_path(uint16_t level_order);
};

#endif // LSM_TREE_H