#ifndef WAL_H
#define WAL_H

#include "../utils/types.hpp"
#include "../red_black_tree/red_black.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

class write_ahead_log {
    
    public:
        write_ahead_log(const std::string& filename);

        ~write_ahead_log();

        void append(const std::string& content);

        void clear();

        void repopulate_memtable(red_black_tree& memtable) const;

    private:
        std::string filename;
        std::ofstream wal_file;
};

#endif // WAL_H