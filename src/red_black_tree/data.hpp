#ifndef DATA_H 
#define DATA_H 

#include "../types.hpp"
#include <string>

struct rb_entry {
    std::string key;
    std::string val;
    int64_t segment;
    int64_t offset;

    // Constructor for generating an entry only with a Key for searching
    rb_entry(std::string key) {
        this->key = key;
    }

    // Constructor for the RB tree of the Memtable
    rb_entry(kv_pair pair) {
        key = pair.key;
        val = pair.val;
        
        // Tombstone Values for segment and offset since they are not used
        // for the memtable.
        segment = -1;
        offset = -1;
    }

    // Constructor for the RB tree of the Sparse Index 
    rb_entry(kv_pair pair, int64_t segment, int64_t offset) {
        key = pair.key;
        val = pair.val;

        this->segment = segment;
        this->offset = offset;
    }

    rb_entry(){};

    bool operator!=(const rb_entry& other) const {return key != other.key;}
    bool operator==(const rb_entry& other) const {return key == other.key;}
    bool operator<(const rb_entry& other) const {return key < other.key;}
    bool operator>(const rb_entry& other) const {return key > other.key;}
};

#endif // DATA_H