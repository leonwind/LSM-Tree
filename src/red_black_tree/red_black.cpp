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
        root->remove({target, ""});
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
        return x == NULL ? NULL : x->pair.key;
    }
    return NULL; 
}

void red_black_tree::print() const {
    root->print_2d(0);
    std::cout << std::endl;
}