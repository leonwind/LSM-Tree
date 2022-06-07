#include "lsm_tree.hpp"

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
    if (memtable.size() >= MEMTABLE_SIZE) {
        compact();
        std::cout << "FINISHED COMPACTION" << std::endl;
        flush_memtable_to_disk();
        std::cout << "FLUSHED MEMTABLE TO DISK" << std::endl;
        wal.clear();
    }

    kv_pair entry = {key, value};
    wal.append(entry.to_log_entry());
    memtable.insert(entry);
}

/**
 * Get a kv-pair from the database.
 * 1. Check if the key is in the memtable.
 * 2. If not, check each segment individually.
 */
std::string lsm_tree::get(const std::string& key) {
    std::optional<std::string> memtable_val = memtable.get(key);
    if (memtable_val.has_value()) {
        return memtable_val.value() == TOMBSTONE ? "" : memtable_val.value();
    }

    std::optional<std::string> segment_val = search_all_segments(key);
    if (segment_val.has_value()) {
        return segment_val.value() == TOMBSTONE ? "" : segment_val.value();
    }

    return "";
}

/**
 * To remove a value from the database, we just insert the key
 * with the TOMBSTONE value.
 * Since we are searching from Memtable -> latest segment, retrieval will find
 * the {key, TOMBSTONE} pair first and knows it got deleted.
 */
void lsm_tree::remove(const std::string& key) {
    put(key, TOMBSTONE);
}

/**
 * Clears the Memtable, the WAL, and all SSTs on the disk.
 */
void lsm_tree::drop_table() {
    memtable.delete_tree();
    wal.clear();

    level::delete_all_segments(SEGMENT_BASE);
    segments.clear();
}

/**
 * Delete the in memory table and store all key values pair in a sorted
 * order in a new segment file.
 */
void lsm_tree::flush_memtable_to_disk() {
    level sst = level(get_new_segment_path(0), (long) memtable.size(), memtable);
    std::cout << "Created new SST" << std::endl;

    if (not segments.empty() and segments.front().first == 0) {
        segments.front().second.push_back(sst);
    } else {
        segments.push_front({0, {sst}});
    }
}

/**
 * Merge multiple segments into one.
 * Start with level i = 0 (smallest SST) and merge 2 SSTs at each level and
 * push the merged one to the next level i + 1.
 */
void lsm_tree::compact() {
    for (auto it = segments.begin(); it != segments.end(); ++it) {
        auto& curr_level = it->first;
        auto& level_segments = it->second;

        while (level_segments.size() >= 2) {
            level sst_a = level_segments.back();
            level_segments.pop_back();

            level sst_b = level_segments.back();
            level_segments.pop_back();

            level merged = level(get_new_segment_path(curr_level + 1), sst_a, sst_b, (long) memtable.size() * (curr_level + 1) * 2);

            sst_a.delete_segment_file();
            sst_b.delete_segment_file();

            auto successor = std::next(it);
            if (successor != segments.end() and successor->first == curr_level + 1) {
                successor->second.push_back(merged);
            } else {
                segments.insert(successor, {curr_level + 1, {merged}});
            }
        }
    }
}

std::optional<std::string> lsm_tree::search_all_segments(const std::string& target) {
    for (const auto& curr_segment : segments) {
        std::cout << "Search on level " << curr_segment.first << std::endl;
        for (const level& sst : curr_segment.second) {
            std::cout << "Search segment " << sst.get_name() << std::endl;
            std::optional<std::string> val = sst.search(target);
            if (val.has_value()) {
                return val;
            }
        }
    }

    return {};
}

/**
 * Restore the memtable and the SSTs after restarting the db.
 */
void lsm_tree::restore_db() {
    restore_memtable();
    restore_segments();
}

/**
 * Restore a memtable from its WAL. 
 */
void lsm_tree::restore_memtable() {
    wal.repopulate_memtable(memtable);
}

/**
 * Restore all segment files from disk.
 */
void lsm_tree::restore_segments() {
    auto last_segment_i_and_segments = level::collect_levels(SEGMENT_BASE, MEMTABLE_SIZE);
    segment_i = last_segment_i_and_segments.first + 1;
    segments = last_segment_i_and_segments.second;
}

/*
 * Create a new unique segment path.
 */
std::string lsm_tree::get_new_segment_path(uint16_t level_order) {
    return SEGMENT_BASE + level::create_filename_based_on_level(segment_i++, level_order) + ".segment";
}
