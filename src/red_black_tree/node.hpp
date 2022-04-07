#ifndef NODE_H
#define NODE_H 

#include "../utils/types.hpp"
#include <string>
#include <vector>

#define RED "RED"
#define BLACK "BLACK"
#define NIL "NIL"

class node {
    public:
        entry pair;
        std::string color;
        node* left;
        node* right;

        node(entry pair);

        void insert(entry new_pair);

        void remove(entry target);

        bool contains(entry target);

        std::vector<entry> inorder();

        void delete_tree(); 

    private:
};

#endif // NODE_H