#ifndef TYPES_H
#define TYPES_H

#include <string>

// Bad solution since this value is not allowed to be used by the user
#define TOMBSTONE "+++TOMBSTONES+++"

struct kv_pair {
    std::string key;
    std::string val;

    bool operator!=(const kv_pair& other) const {return key != other.key;}
    bool operator==(const kv_pair& other) const {return key == other.key;}
    bool operator<(const kv_pair& other) const {return key < other.key;}
    bool operator<=(const kv_pair& other) const {return key <= other.key;}
    bool operator>(const kv_pair& other) const {return key > other.key;}
    bool operator>=(const kv_pair& other) const {return key >= other.key;}

    /*
    bool operator!=(const entry& other) const {return std::stoi(key) != std::stoi(other.key);}
    bool operator==(const entry& other) const {return std::stoi(key) == std::stoi(other.key);}
    bool operator<(const entry& other) const {return std::stoi(key) < std::stoi(other.key);}
    bool operator>(const entry& other) const {return std::stoi(key) > std::stoi(other.key);}
    */
};

#endif // TYPES_H