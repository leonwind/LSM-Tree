#include "lsm_tree.hpp"
#include <iostream>
#include <string>

const std::string lsm_tree::MEMTABLE_PATH = "storage/memtable.bckup";
const std::string lsm_tree::WAL_PATH = "../src/storage/wal.log";
const std::string lsm_tree::SEGMENT_BASE = "../src/storage/segments/";


lsm_tree::lsm_tree(): bloom(BLOOM_SIZE), memtable(), index(), wal(WAL_PATH) {
    segment_i = 0; 
}

lsm_tree::~lsm_tree() {}

void lsm_tree::put(std::string key, std::string value) {
    entry kv_pair = {key, value};

    if (memtable.size + key.size() + value.size() > MEMTABLE_SIZE) {
        compact();
        flush_memtable_to_disk();
        wal.clear();
    }

    wal.append(pair_to_log_entry(kv_pair));
    memtable.insert(kv_pair);
}

std::string lsm_tree::get(std::string key) {
    std::string val = memtable.get(key);
    if (val != "") {
        return val;
    }

    if (!bloom.is_set(key)) {
        return "";
    }

    // Check the index now to search on the disk
    return "";
}

void lsm_tree::remove(std::string key) {
    memtable.remove(key);
}

std::vector<entry> lsm_tree::range(std::string start, size_t len) {
    std::vector<entry> pairs;
    return pairs;
}

void lsm_tree::compact() {

}

void lsm_tree::flush_memtable_to_disk() {
    std::ofstream segment;
    segment.open(new_segment_path());
    std::vector<entry> pairs = memtable.get_and_delete_all_nodes();
    std::cout << "HERE" << std::endl;

    for (auto& pair : pairs) {
        bloom.set(pair.key);

        segment << pair_to_log_entry(pair) << std::flush;
    } 
}

void lsm_tree::restore_memtable() {

}

std::string lsm_tree::new_segment_path() {
    return SEGMENT_BASE + std::to_string(++segment_i);
}

std::string lsm_tree::pair_to_log_entry(entry pair) {
    return pair.key + "," + pair.val + "\n";
}
