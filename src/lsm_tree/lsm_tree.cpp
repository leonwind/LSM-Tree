#include "lsm_tree.hpp"
#include <string>

lsm_tree::lsm_tree(): memtable(), wal(WAL_PATH) {
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

    if (memtable.size >= 2) {
        compact();
        std::cout << "FINISHED COMPACTION" << std::endl;
        flush_memtable_to_disk();
        std::cout << "FLUSHED MEMTABLE TO DISK" << std::endl;
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
    std::optional<std::string> val = memtable.get(key);

    if (not val.has_value()) {
        std::optional<std::string> ans = search_all_segments(key);
        return ans.has_value() ? ans.value() : "";
    }

    if (val.value() == TOMBSTONE) {
        return "";
    }

    return val.value();
}

void lsm_tree::remove(const std::string& key) {
    put(key, TOMBSTONE);
}

/**
 * Clears the Memtable, the WAL, and all SSTs on the disk.
 */
void lsm_tree::drop_table() {
    memtable.delete_tree();
    level::delete_all_segments(SEGMENT_BASE);
    wal.clear();
}

/**
 * Merge multiple segments into one.
 * Start with level i = 0 (smallest SST) and merge 2 SSTs at each level and
 * push the merged one to the next level i + 1.
 */
void lsm_tree::compact() {
    // TODO: Store level index seperately since levels can be missing after
    // merging, e.g. level 0 and level 2 exists but not level 1
    size_t num_levels = segments.size();

    for (size_t i = 0; i < num_levels; i++) {
        if (not segments.contains(i)) {
            continue;
        }
        auto& level_segments = segments[i];

        while (level_segments.size() >= 2) {
            level sst_a = level_segments.back();
            level_segments.pop_back();

            level sst_b = level_segments.back();
            level_segments.pop_back();

            level merged = level(get_new_segment_path(segment_i++), sst_a, sst_b, memtable.size * (i + 1) * 2);

            sst_a.delete_segment_file();
            sst_b.delete_segment_file();

            if (segments.contains(i + 1)) {
                segments[i + 1].push_back(merged);
            } else {
                segments[i + 1] = {merged};
            }
        }
    }
}

/**
 * Delete the in memory table and store all key values pair in a sorted
 * order in a new segment file.
 */
void lsm_tree::flush_memtable_to_disk() {
    // TODO: Size stuff
    level sst = level(get_new_segment_path(segment_i), memtable.size, memtable);

    if (segments.contains(0)) {
        segments[0].push_back(sst);
    } else {
        segments[0] = {sst};
    }
    segment_i++;
}

std::optional<std::string> lsm_tree::search_all_segments(const std::string& target) {
    size_t num_levels = segments.size();
    for (size_t i = 0; i < num_levels; i++) {
        std::cout << "CHECK LEVEL i = " << i << std::endl;
        for (level& sst : segments[i]) {
            std::optional<std::string> val = sst.search(target);
            if (val.has_value()) {
                return val;
            }
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
