#ifndef NODE_H
#define NODE_H 

#include "../utils/types.hpp"
#include "data.hpp"
#include <string>
#include <vector>


class node {
    enum colors {RED, BLACK, DOUBLE_BLACK};
    
    public:
        rb_entry pair;
        colors color;

        node* left;
        node* right;
        node* parent;

        node(const rb_entry& data, bool is_root);

        explicit node(node* ptr);

        int64_t insert(const rb_entry& new_pair);

        bool remove(node* to_remove);

        node* find_node(const rb_entry& target);

        node* floor(const rb_entry& target);

        void in_order(std::vector<rb_entry>& nodes, bool delete_node) const;

        void delete_tree(); 

        void print_2d(int space) const;

        std::string to_str() const;

    private:
        std::pair<int64_t, node*> insert_node(const rb_entry& new_pair);

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