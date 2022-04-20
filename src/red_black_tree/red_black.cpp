#include "red_black.hpp"
#include "node.hpp"
#include <iostream>

red_black_tree::red_black_tree() {
    root = NULL;
    size = 0;
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
    // TODO: Only update the size if the key is not in the tree yet, otherwise only
    // update with difference between the old and the new value.
    size += new_pair.key.size() + new_pair.val.size();
}

void red_black_tree::remove(std::string target) {
    if (root != NULL) {
        bool root_deleted = root->remove({target, ""});
        if (root_deleted) {
            root = NULL;
        }
    }
    // Just an approximation since currently we do not know the size of the value of the pair
    // TODO: Change that for actual size as well as only delete if the key exists. 
    size -= target.size() * 2;
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
    root->in_order(nodes, false);
    return nodes;
}

std::vector<entry> red_black_tree::get_and_delete_all_nodes() {
    std::vector<entry> nodes;
    root->in_order(nodes, true);
    size = 0;
    root = NULL;
    return nodes;
}

void red_black_tree::print() const {
    if (root != NULL) {
        root->print_2d(0);
        std::cout << std::endl;
    }
}