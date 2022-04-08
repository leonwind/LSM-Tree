#ifndef TYPES_H
#define TYPES_H

#include <string>
#define KEY_MAX 2147483647
#define KEY_MIN -2147483648

#define VAL_MAX 2147483647
#define VAL_MIN -2147483647

struct entry {
    std::string key;
    std::string val;

    bool operator==(const entry& other) const {return key == other.key;}
    bool operator<(const entry& other) const {return key < other.key;}
    bool operator>(const entry& other) const {return key > other.key;}
};

typedef struct entry entry_t;

#endif // TYPES_H