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

        bool remove(entry target);

        node* find_node(entry target);

        void in_order(std::vector<entry>& nodes) const;

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

        void check_for_errors(bool is_root);
};

#endif // NODE_H