#ifndef RED_BLACK_H 
#define RED_BLACK_H

#include "node.hpp"
#include "../utils/types.hpp"

class red_black_tree {
    public:
        red_black_tree();

        ~red_black_tree();

        void insert(entry pair);

        void remove(entry pair);

        bool contains(entry pair);

        void print();

    private:
        node* root;

};

#endif // RED_BLACK_H