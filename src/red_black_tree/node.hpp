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
        node* insert_node(entry new_pair);

        void fix_insert();

        node* remove_node(entry target);

        void fix_remove();

        node* find_min();

        void rotate_left();

        void rotate_right();

        void print_seperator();
};

#endif // NODE_H