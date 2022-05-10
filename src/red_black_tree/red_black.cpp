#include "red_black.hpp"
#include "data.hpp"
#include "node.hpp"
#include <cstdint>
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

// Insert method for the Memtable RB tree
void red_black_tree::insert(kv_pair new_pair) {
    rb_entry data = rb_entry{new_pair};
    insert(data); 
}

// Insert method for the Sparse Index RB tree
void red_black_tree::insert(kv_pair new_pair, int64_t segment, int64_t offset) {
    rb_entry data = rb_entry{new_pair, segment, offset};
    insert(data);
}

void red_black_tree::insert(rb_entry data) {
    if (root == NULL) {
        root = new node(data, true);
    } else {
        root->insert(data); 
    }
    // TODO: Only update the size if the key is not in the tree yet, otherwise only
    // update with difference between the old and the new value.
    size += data.key.size() + data.val.size();
}

void red_black_tree::remove(std::string target) {
    if (root != NULL) {
        bool root_deleted = root->remove({target});
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
        return root->find_node({target}) != NULL;
    }
    return false;
}

std::string red_black_tree::get(std::string target) const {
    if (root != NULL) {
        node* x = root->find_node({target});
        return x == NULL ? TOMBSTONE : x->pair.val; 
    }
    return TOMBSTONE;
}

// Return the node which is the next smallest one if target does not exist
rb_entry red_black_tree::floor(std::string target) const {
    if (root != NULL) {
        node* x = root->floor({target});
        return x == NULL ? rb_entry{TOMBSTONE} : x->pair;
    }
    return rb_entry{TOMBSTONE};
}

std::vector<rb_entry> red_black_tree::get_all_nodes() const {
    std::vector<rb_entry> nodes;
    root->in_order(nodes, false);
    return nodes;
}

std::vector<rb_entry> red_black_tree::get_and_delete_all_nodes() {
    std::vector<rb_entry> nodes;
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