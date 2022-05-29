#include "lsm_tree.hpp"
#include <string>

const std::string BASE{"../src/.internal_storage/"};
const std::string lsm_tree::MEMTABLE_PATH{BASE + "memtable.bckup"};
const std::string lsm_tree::WAL_PATH{BASE + "wal.log"};
const std::string lsm_tree::SEGMENT_BASE{BASE + "segments/"};


lsm_tree::lsm_tree(): memtable(), wal(WAL_PATH) {
    segment_i = 0;
    restore_db();
}

lsm_tree::~lsm_tree() = default;

/**
 * Insert a new kv-pair into the database:
 * 1. If the size of the memtable is greater than MEMTABLE_SIZE, flush the 
 * memtable to the disk and create a new segment with the pairs on disk,
 * and possibly merge multiple segments into one. Reset the WAL.
 * 2. Write kv-pair to WAL.
 * 3. Insert into memtable.
 */
void lsm_tree::put(const std::string& key, const std::string& value) {
    kv_pair entry = {key, value};

    if (memtable.size + key.size() + value.size() > MEMTABLE_SIZE) {
        compact();
        flush_memtable_to_disk();
        wal.clear();
    }

    wal.append(entry.to_log_entry());
    memtable.insert(entry);
}

/**
 * Get a kv-pair from the database.
 * 1. Check if the key is in the memtable.
 * 2. If not, check each segment individually using a bloom filter quickly.
 */
std::string lsm_tree::get(const std::string& key) {
    std::string val = memtable.get(key);
    if (val == TOMBSTONE) {
        return "";
    }

    if (!val.empty()) {
        return val;
    }

    std::optional<std::string> ans = search_all_segments(key);
    return ans.has_value() ? ans.value() : "";
}

void lsm_tree::remove(const std::string& key) {
    put(key, TOMBSTONE);
}

/**
 * Clears the whole store, Memtable and on-disk segments.
 * TODO
 */
void lsm_tree::clear() {

}

/**
 * Merge multiple segments into one.
 * TODO
 */
void lsm_tree::compact() {

}

/**
 * Delete the in memory table and store all key values pair in a sorted
 * order in a new segment file.
 */
void lsm_tree::flush_memtable_to_disk() {
    level sst = level(get_new_segment_path(segment_i), memtable.size, memtable);
    segments.push_back(sst);
    segment_i++;
}

std::optional<std::string> lsm_tree::search_all_segments(const std::string& target) {
    for (level &sst : segments) {
        std::optional<std::string> val = sst.search(target);
        if (val.has_value()) {
            return val;
        }
    }
    return {};
}

/**
 * Restore the in-memory tree, the bloom filter, the sparse index,
 * and all the segments after restarting the db
 */
void lsm_tree::restore_db() {
    restore_memtable();
    restore_segments();
}

/**
 * Restore a memtable from its WAL. 
 * TODO
 */
void lsm_tree::restore_memtable() {

}

/**
 * Restore all segment files from disk.
 */
void lsm_tree::restore_segments() {

}

/*
 * Create a new unique segment path.
 * TODO
 */
std::string lsm_tree::get_new_segment_path(int64_t i) {
    std::string path{SEGMENT_BASE + std::to_string(i) + ".segment"};
    return path;
}
