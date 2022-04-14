#ifndef NODE_H
#define NODE_H 

#include "../types.hpp"
#include <string>
#include <vector>


class node {
    enum colors {RED, BLACK, DOUBLE_BLACK};
    
    public:
        entry pair;
        colors color;

        node* left;
        node* right;
        node* parent;

        node(entry pair, bool is_root);

        node(node* ptr);

        void insert(entry new_pair);

        void remove(entry target);

        node* find_node(entry target);

        std::vector<entry> get_all_nodes() const;

        void delete_tree(); 

        void print_2d(int space) const;

        std::string to_str() const;

    private:
        node* insert_node(entry new_pair);

        void fix_insert();

        void remove_node();

        node* find_min();

        void rotate_left();

        void rotate_right();

        void print_seperator();

        static colors get_color(node* curr);

        static void set_color(node* curr, colors new_color);
};

#endif // NODE_H