#ifndef LSM_TREE_H
#define LSM_TREE_H

#include "../utils/types.hpp"
#include "../bloom_filter/bloom.hpp"
#include "../red_black_tree/red_black.hpp"
#include "../wal/wal.hpp"
#include "level/level.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

class lsm_tree {

    public:
        lsm_tree();

        ~lsm_tree();
        
        void put(const std::string& key, const std::string& value);

        std::string get(const std::string& key);

        void remove(const std::string& key);

        void clear();

    private:
        static const uint64_t MEMTABLE_SIZE{1000};
        static const std::string MEMTABLE_PATH;
        static const std::string WAL_PATH;
        static const std::string SEGMENT_BASE;

        red_black_tree memtable;
        std::vector<level> segments;
        write_ahead_log wal;
        int64_t segment_i;

        void flush_memtable_to_disk();

        void restore_db();

        void restore_memtable();

        void restore_segments();

        void compact();

        std::optional<std::string> search_all_segments(const std::string& target);

        static std::string get_new_segment_path(int64_t i);
};

#endif // LSM_TREE_H