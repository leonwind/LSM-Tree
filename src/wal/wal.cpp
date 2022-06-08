#include "wal.hpp"

write_ahead_log::write_ahead_log(const std::string& filename) {
    this->filename = filename;
    wal_file.open(filename, std::ios::in | std::ios::out | std::ios::app);
}

write_ahead_log::~write_ahead_log() = default;

void write_ahead_log::append(const std::string& content) {
    if (wal_file.is_open()) {
        wal_file << content;
    }
}

void write_ahead_log::clear() {
    std::filesystem::resize_file(filename, 0);
}

void write_ahead_log::repopulate_memtable(red_black_tree& memtable) const {
    std::ifstream wal_tmp;
    wal_tmp.open(filename);

    std::string line;
    std::string key, value;

    while (std::getline(wal_tmp, line)) {
        size_t seperator_pos = line.find(SEPERATOR);
        key = line.substr(0, seperator_pos);
        value = line.substr(seperator_pos + 1, line.size());

        memtable.insert(kv_pair{key, value});
    }

    wal_tmp.close();
}