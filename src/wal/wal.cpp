#include "wal.hpp"

write_ahead_log::write_ahead_log(std::string filename) {
    std::cout << "WAL: " << filename << std::endl;
    this->filename = filename;
    wal_file.open(filename, std::ios_base::app);
}

write_ahead_log::~write_ahead_log() {}

void write_ahead_log::append(std::string content) {
    if (wal_file) {
        wal_file << content << std::flush;
        std::cout << "Added " << content << std::endl;
    } else {
        std::cout << "Unable to open the write ahead log file" << std::endl;
    }
}

void write_ahead_log::clear() {
    std::filesystem::resize_file(filename, 0);
}