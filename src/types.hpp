#ifndef TYPES_H
#define TYPES_H

#include <string>

// Bad solution since this value is not allowed to be used by the user
// of the db.
#define TOMBSTONE "+++TOMBSTONES+++"

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