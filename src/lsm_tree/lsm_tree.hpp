#ifndef LSM_TREE_H
#define LSM_TREE_H

#include "../types.hpp"
#include "../bloom_filter/bloom.hpp"
#include "../red_black_tree/red_black.hpp"
#include "../wal/wal.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class lsm_tree {

    public:
        lsm_tree();

        ~lsm_tree();
        
        void put(std::string key, std:: string value);

        std::string get(std::string key);

        void remove(std::string key);

        std::vector<entry> range(std::string start, size_t length);

    private:
        static const long BLOOM_SIZE = 1000000;
        static const uint64_t MEMTABLE_SIZE = 1000;
        static const std::string MEMTABLE_PATH;
        static const std::string WAL_PATH;
        static const std::string SEGMENT_BASE;

        bloom_filter bloom; 
        red_black_tree memtable;
        red_black_tree index;
        std::vector<std::string> segments;
        write_ahead_log wal;

        std::string segment_path;
        uint segment_i;

        void flush_memtable_to_disk();

        void restore_memtable();

        void compact();

        std::string pair_to_log_entry(entry pair);

        std::string new_segment_path();
};

#endif // LSM_TREE_H