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

        node(entry pair, bool is_root);

        void insert(entry new_pair);

        void remove(entry target);

        bool exists(entry target) const;

        std::vector<entry> get_all_nodes() const;

        void delete_tree(); 

        void print_2d(int space) const;

        std::string to_str() const;

    private:
        node* bst_insertion(entry new_pair);

        void fix_insert();

        void fix_remove();

        void rotate_left();

        void rotate_right();

        void print_seperator();
};

#endif // NODE_H