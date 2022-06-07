#include "red_black.hpp"
#include "data.hpp"
#include "node.hpp"
#include <iostream>
#include <utility>

red_black_tree::red_black_tree() {
    root = nullptr;
    size = 0;
}

red_black_tree::~red_black_tree() {
   delete_tree();
}

void red_black_tree::delete_tree() {
    if (root != nullptr) {
        root->delete_tree();
    }

    root = nullptr;
    size = 0;
}

// Insert method for the Memtable RB tree
void red_black_tree::insert(kv_pair new_pair) {
    rb_entry data = rb_entry{std::move(new_pair)};
    insert(data); 
}

void red_black_tree::insert(const rb_entry& data) {
    if (root == nullptr) {
        root = new node(data, true);
    } else {
        root->insert(data); 
    }
    // TODO: Only update the size if the key is not in the tree yet, otherwise only
    // update with difference between the old and the new value.
    size += data.key.size();// + data.val.size();
}

void red_black_tree::remove(const std::string& target) {
    if (root != nullptr) {
        bool root_deleted = root->remove({target, ""});
        if (root_deleted) {
            root = nullptr;
        }
    }

    // Just an approximation since currently we do not know the size of the value of the pair
    // TODO: Change that for actual size as well as only delete if the key exists. 
    size -= target.size() * 2;
}

bool red_black_tree::exists(std::string target) const {
    if (root != nullptr) {
        return root->find_node(rb_entry{std::move(target)}) != nullptr;
    }
    return false;
}

std::optional<std::string> red_black_tree::get(std::string target) const {
    if (root != nullptr) {
        node* x = root->find_node(rb_entry{std::move(target)});
        if (x == nullptr) {
            return {};
        }
        return std::any_cast<std::string>(x->pair.val.value());
    }
    return {};
}

// Return the node which is the next smallest one if target does not exist
std::optional<rb_entry> red_black_tree::floor(std::string target) const {
    if (root != nullptr) {
        node* x = root->floor(rb_entry{std::move(target)});
        if (x == nullptr) {
            return {};
        }
        return x->pair;
    }
    return {};
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
    root = nullptr;

    return nodes;
}

void red_black_tree::print() const {
    if (root != nullptr) {
        root->print_2d(0);
        std::cout << std::endl;
    }
}