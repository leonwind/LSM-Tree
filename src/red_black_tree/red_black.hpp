#ifndef RED_BLACK_H 
#define RED_BLACK_H

#include "data.hpp"
#include "node.hpp"
#include "../types.hpp"
#include <cstdint>
#include <vector>

class red_black_tree {

    public:
        uint64_t size;

        red_black_tree();

        ~red_black_tree();

        void insert(kv_pair new_pair);

        void insert(kv_pair new_pair, int64_t segment, int64_t offset);

        void remove(std::string target);

        bool exists(std::string target) const;

        std::string get(std::string target) const;

        rb_entry floor(std::string target) const;

        std::vector<rb_entry> get_all_nodes() const;

        std::vector<rb_entry> get_and_delete_all_nodes();

        void print() const;

    private:
        node* root;

        void insert(rb_entry data);
};

#endif // RED_BLACK_H