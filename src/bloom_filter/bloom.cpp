#include "bloom.hpp"

/*
 * Implementation of a Bloom filter using k = 3 hash functions.
 * The Hash functions are taken from https://gist.github.com/badboy/6267743
 */


bloom_filter::bloom_filter(long length): table(length) {}

bloom_filter::~bloom_filter() {
    ~table;
}

uint64_t bloom_filter::hash_1(uint64_t key) const {
    key = ~key + (key<<15);
    key = key ^ (key>>12);
    key = key + (key<<2);
    key = key ^ (key>>4);
    key = key * 2057;
    key = key ^ (key>>16);

    return key % table.size();
}

uint64_t bloom_filter::hash_2(uint64_t key) const {
    key = (key+0x7ed55d16) + (key<<12);
    key = (key^0xc761c23c) ^ (key>>19);
    key = (key+0x165667b1) + (key<<5);
    key = (key+0xd3a2646c) ^ (key<<9);
    key = (key+0xfd7046c5) + (key<<3);
    key = (key^0xb55a4f09) ^ (key>>16);

    return key % table.size();
}

uint64_t bloom_filter::hash_3(uint64_t key) const {
    key = (key^61) ^ (key>>16);
    key = key + (key<<3);
    key = key ^ (key>>4);
    key = key * 0x27d4eb2d;
    key = key ^ (key>>15);

    return key % table.size();
}

void bloom_filter::set(KEY_t key) {
    table.set(hash_1(key));
    table.set(hash_2(key));
    table.set(hash_3(key));
}

bool bloom_filter::is_set(KEY_t key) const {
    return table.test(hash_1(key)) 
        && table.test(hash_2(key)) 
        && table.test(hash_3(key));
}
