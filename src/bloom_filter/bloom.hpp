#ifndef BLOOM_H
#define BLOOM_H

#include "../utils/types.hpp"
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

class bloom_filter {
    boost::dynamic_bitset<> table;

    public:
        bloom_filter(long length);

        ~bloom_filter();

        void set(KEY_t);

        bool is_set(KEY_t) const;

    private:
        uint64_t hash_1(uint64_t) const;

        uint64_t hash_2(uint64_t) const;

        uint64_t hash_3(uint64_t) const;
};

#endif //BLOOM_H