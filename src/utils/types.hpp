#ifndef TYPES_H
#define TYPES_H

#include <string>

// Bad solution since this value is not allowed to be used by the user
#define TOMBSTONE "+++TOMBSTONE+++"
#define SEPERATOR ','

struct kv_pair {
    std::string key;
    std::string val;

    bool operator!=(const kv_pair& other) const {return key != other.key;}
    bool operator==(const kv_pair& other) const {return key == other.key;}
    bool operator<(const kv_pair& other) const {
        if (key == other.key) {
            if (val == TOMBSTONE) {
                return true;
            }
            if (other.val == TOMBSTONE) {
                return false;
            }
        }
        return key < other.key;
    }
    bool operator<=(const kv_pair& other) const {return key <= other.key;}
    bool operator>(const kv_pair& other) const {return key > other.key;}
    bool operator>=(const kv_pair& other) const {return key >= other.key;}

    /*
    bool operator!=(const entry& other) const {return std::stoi(key) != std::stoi(other.key);}
    bool operator==(const entry& other) const {return std::stoi(key) == std::stoi(other.key);}
    bool operator<(const entry& other) const {return std::stoi(key) < std::stoi(other.key);}
    bool operator>(const entry& other) const {return std::stoi(key) > std::stoi(other.key);}
    */

    std::string to_log_entry() const {
        return key + SEPERATOR + val + "\n";
    }
};

#endif // TYPES_H