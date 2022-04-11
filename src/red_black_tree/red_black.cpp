#include "red_black.hpp"
#include "node.hpp"
#include <iostream>

red_black_tree::red_black_tree() {
    root = NULL;
}

red_black_tree::~red_black_tree() {}

void red_black_tree::insert(entry new_pair) {
    if (root == NULL) {
        root = new node(new_pair, true);
    } else {
        root->insert(new_pair); 
    }
}

void red_black_tree::remove(entry target) {

}

bool red_black_tree::exists(entry target) const {
    return root->exists(target);
}

void red_black_tree::print() const {
    root->print_2d(0);
    std::cout << std::endl;
}