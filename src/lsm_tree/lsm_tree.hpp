#ifndef LSM_TREE_H
#define LSM_TREE_H

#include "../types.hpp"
#include "../bloom_filter/bloom.hpp"
#include "../red_black_tree/red_black.hpp"
#include <string>
#include <vector>

class lsm_tree {

    public:
        lsm_tree();

        ~lsm_tree();

        void put(std::string key, std:: string value);

        std::string get(std::string key);

        void remove(std::string key);

        std::vector<entry> range(std::string start, size_t length);

    private:
        static const long BLOOM_SIZE = 1000;

        bloom_filter bloom; 
        red_black_tree rb_tree;
};

#endif // LSM_TREE_H