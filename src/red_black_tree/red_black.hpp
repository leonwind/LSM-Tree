#ifndef RED_BLACK_H 
#define RED_BLACK_H

#include "node.hpp"
#include "../types.hpp"
#include <vector>

class red_black_tree {

    public:
        red_black_tree();

        ~red_black_tree();

        void insert(entry new_pair);

        void remove(std::string target);

        bool exists(std::string target) const;

        std::string get(std::string target) const;

        std::vector<entry> get_all_nodes() const;

        void print() const;

    private:
        node* root;
};

#endif // RED_BLACK_H