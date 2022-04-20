#ifndef WAL_H
#define WAL_H

#include "../types.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

class write_ahead_log {
    
    public:
        write_ahead_log(std::string filename);

        ~write_ahead_log();

        void append(std::string content);

        void clear();

    private:
        std::string filename;
        std::ofstream wal_file;
};

#endif // WAL_H