#ifndef NODE_H
#define NODE_H 

#include "../types.hpp"
#include <string>
#include <vector>


class node {
    enum colors {RED, BLACK};
    
    public:
        entry pair;
        colors color;

        node* left;
        node* right;
        node* parent;

        node(entry pair);

        void insert(entry new_pair);

        void remove(entry target);

        bool exists(entry target) const;

        std::vector<entry> inorder() const;

        void delete_tree(); 

    private:
        void bst_insertion(entry new_pair);

        void fix_violations();

        void rotate_left();

        void rotate_right();

        void recolor();
};

#endif // NODE_H