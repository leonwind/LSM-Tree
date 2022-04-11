#ifndef RED_BLACK_H 
#define RED_BLACK_H

#include "node.hpp"
#include "../types.hpp"

class red_black_tree {

    public:
        red_black_tree();

        ~red_black_tree();

        void insert(entry new_pair);

        void remove(entry target);

        bool exists(entry target) const;

        void print() const;

    private:
        node* root;
};

#endif // RED_BLACK_H