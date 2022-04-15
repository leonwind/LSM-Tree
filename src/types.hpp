#ifndef TYPES_H
#define TYPES_H

#include <string>

struct entry {
    std::string key;
    std::string val;

    bool operator!=(const entry& other) const {return key != other.key;}
    bool operator==(const entry& other) const {return key == other.key;}
    bool operator<(const entry& other) const {return key < other.key;}
    bool operator>(const entry& other) const {return key > other.key;}
    
    /*
    bool operator!=(const entry& other) const {return std::stoi(key) != std::stoi(other.key);}
    bool operator==(const entry& other) const {return std::stoi(key) == std::stoi(other.key);}
    bool operator<(const entry& other) const {return std::stoi(key) < std::stoi(other.key);}
    bool operator>(const entry& other) const {return std::stoi(key) > std::stoi(other.key);}
    */
};

typedef struct entry entry_t;

#endif // TYPES_H