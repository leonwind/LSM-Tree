#include "wal.hpp"
#include <ios>

write_ahead_log::write_ahead_log(std::string filename) {
    this->filename = filename;
    wal_file.open(filename, std::ios_base::app);
}

void write_ahead_log::append(std::string content) {
    if (wal_file) {
        wal_file << std::endl;
        wal_file << std::endl;
    } else {
        std::cout << "Unable to open the write ahead log file" << std::endl;
    }
}

void write_ahead_log::clear() {
    wal_file.open(filename);
    
}