#ifndef LSM_TREE_H
#define LSM_TREE_H

#include "../utils/types.hpp"
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
        
        void put(const std::string& key, const std::string& value);

        std::string get(const std::string& key);

        void remove(const std::string& key);

        void clear();

    private:
        static const uint64_t BLOOM_SIZE{1000000};
        static const uint64_t MEMTABLE_SIZE{1000};
        static const uint64_t SPARSITY_FACTOR{1000};
        static const std::string MEMTABLE_PATH;
        static const std::string WAL_PATH;
        static const std::string SEGMENT_BASE;

        bloom_filter bloom; 
        red_black_tree memtable;
        red_black_tree index;
        std::vector<std::string> segments;
        write_ahead_log wal;

        int64_t segment_i;
        int64_t sparsity_counter;

        void flush_memtable_to_disk();

        void restore_db();

        void restore_memtable();

        void restore_segments();

        void compact();

        std::pair<bool, std::string> search_all_segments(const std::string& target);

        static std::pair<bool, std::string> search_segment(const std::string& target, const std::string& path, int64_t offset);

        static std::string pair_to_log_entry(const kv_pair& entry);

        static std::string get_new_segment_path(int64_t i);

        void reset_sparsity_counter();
};

#endif // LSM_TREE_H