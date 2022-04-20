#include "lsm_tree.hpp"
#include <iostream>
#include <string>

/**
 * Main functionality of the LSM Tree.
 * Supports put, get, and remove operation.
 * Some ideas for the implementation from https://github.com/chrislessard/LSM-Tree/blob/master/src/lsm_tree.py
 * e.g. Sparsity Counter.
 */


const std::string lsm_tree::MEMTABLE_PATH = "../src/.internal_storage/memtable.bckup";
const std::string lsm_tree::WAL_PATH = "../src/.internal_storage/wal.log";
const std::string lsm_tree::SEGMENT_BASE = "../src/.internal_storage/segments/";


lsm_tree::lsm_tree(): bloom(BLOOM_SIZE), memtable(), index(), wal(WAL_PATH) {
    segment_i = 0; 
    sparsity_counter = SPARSITY_FACTOR;
}

lsm_tree::~lsm_tree() {}

/**
 * Insert a new kv-pair into the database:
 * 1. If the size of the memtable is greater than MEMTABLE_SIZE, flush the 
 * memtable to the disk and create a new segment with the pairs on disk,
 * and possibly merge multiple segments into one. Reset the WAL.
 * 2. Write kv-pair to WAL.
 * 3. Insert into memtable.
 */
void lsm_tree::put(std::string key, std::string value) {
    entry kv_pair = {key, value};

    if (memtable.size + key.size() + value.size() > 5) {
        compact();
        flush_memtable_to_disk();
        wal.clear();
    }

    wal.append(pair_to_log_entry(kv_pair));
    memtable.insert(kv_pair);
}

/**
 * Get a kv-pair from the database.
 * 1. Check if the key is in the memtable.
 * 2. If not, check if the key exists in the bloom filter.
 * 3. If the key is set in the bloom filter, use the Sparse Index to find the possible
 * range and search it in there.
 */
std::string lsm_tree::get(std::string key) {
    std::string val = memtable.get(key);
    if (val != "") {
        return val;
    }

    if (!bloom.is_set(key)) {
        return "";
    }

    return "";
}

void lsm_tree::remove(std::string key) {
    memtable.remove(key);
}

std::vector<entry> lsm_tree::range(std::string start, size_t len) {
    std::vector<entry> pairs;
    return pairs;
}

/**
 * Merge multiple segments into one.
 */
void lsm_tree::compact() {

}

/**
 * Delete the in memory table and store all key values pair in a sorted
 * order in a new segment file.
 */
void lsm_tree::flush_memtable_to_disk() {
    std::ofstream segment;
    std::string curr_segment = get_new_segment_path();
    segment.open(curr_segment);
    uint64_t key_offset = 0;

    for (auto& pair : memtable.get_and_delete_all_nodes()) {
        bloom.set(pair.key);

        std::string log_entry = pair_to_log_entry(pair);
        segment << log_entry << std::flush;
        key_offset += log_entry.size();
    }

    segments.push_back(curr_segment);
}

/**
 * Restore a memtable from its backup file.
 */
void lsm_tree::restore_memtable() {

}

/*
 * Create a new unique segment path.
 */
std::string lsm_tree::get_new_segment_path() {
    return SEGMENT_BASE + std::to_string(++segment_i) + ".segment";
}

/**
 * Converts a kv-pair into a comma seperated newline log entry.
 */
std::string lsm_tree::pair_to_log_entry(entry pair) {
    return pair.key + "," + pair.val + "\n";
}

void lsm_tree::reset_sparsity_counter() {
    sparsity_counter = SPARSITY_FACTOR;
}