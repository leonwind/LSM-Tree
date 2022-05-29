#ifndef DATA_H 
#define DATA_H 

#include "../utils/types.hpp"
#include <string>
#include <any>
#include <utility>

struct rb_entry {
    std::string key;
    // val is either a string for the memtable or an uint64_t for the
    // Sparse Index as the offset inside the SST.
    std::any val;

    // Constructor for the RB tree of the Memtable
    explicit rb_entry(kv_pair pair) {
        key = pair.key;
        val = pair.val;
    }

    rb_entry(std::string key, std::any val) {
        this->key = std::move(key);
        this->val = std::move(val);
    }

    //rb_entry()= default;;

    bool operator!=(const rb_entry& other) const {return key != other.key;}
    bool operator==(const rb_entry& other) const {return key == other.key;}
    bool operator<(const rb_entry& other) const {return key < other.key;}
    bool operator<=(const rb_entry& other) const {return key <= other.key;}
    bool operator>(const rb_entry& other) const {return key > other.key;}
    bool operator>=(const rb_entry& other) const {return key >= other.key;}
};

#endif // DATA_H
