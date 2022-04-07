#ifndef BLOOM_H
#define BLOOM_H


#include <boost/dynamic_bitset/dynamic_bitset.hpp>

class bloom_filter {
    boost::dynamic_bitset<> table;

    public:
        bloom_filter(long length);

        ~bloom_filter();

        void set(u_int64_t);

        bool is_set(u_int64_t) const;

    private:
        u_int64_t hash_1(u_int64_t) const;

        u_int64_t hash_2(u_int64_t) const;

        u_int64_t hash_3(u_int64_t) const;
};

#endif //BLOOM_H