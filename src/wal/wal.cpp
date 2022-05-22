#include "wal.hpp"

write_ahead_log::write_ahead_log(const std::string& filename) {
    this->filename = filename;
    wal_file.open(filename, std::ios_base::app);
}

write_ahead_log::~write_ahead_log() = default;

void write_ahead_log::append(const std::string& content) {
    if (wal_file) {
        wal_file << content << std::flush;
    }
}

void write_ahead_log::clear() {
    std::filesystem::resize_file(filename, 0);
}