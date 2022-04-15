#include "red_black.hpp"
#include "node.hpp"
#include <iostream>

red_black_tree::red_black_tree() {
    root = NULL;
}

red_black_tree::~red_black_tree() {
    if (root != NULL) {
        root->delete_tree();
    }
}

void red_black_tree::insert(entry new_pair) {
    if (root == NULL) {
        root = new node(new_pair, true);
    } else {
        root->insert(new_pair); 
    }
}

void red_black_tree::remove(std::string target) {
    if (root != NULL) {
        bool root_deleted = root->remove({target, ""});
        if (root_deleted) {
            root = NULL;
        }
    }
}

bool red_black_tree::exists(std::string target) const {
    if (root != NULL) {
        return root->find_node({target, ""}) != NULL;
    }
    return false;
}

std::string red_black_tree::get(std::string target) const {
    if (root != NULL) {
        node* x = root->find_node({target, ""});
        return x == NULL ? "" : x->pair.val; 
    }
    return "";
}

std::vector<entry> red_black_tree::get_all_nodes() const {
    std::vector<entry> nodes;
    root->in_order(nodes);
    return nodes;
}

void red_black_tree::print() const {
    if (root != NULL) {
        root->print_2d(0);
        std::cout << std::endl;
    }
}