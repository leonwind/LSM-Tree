#include "red_black.hpp"
#include "data.hpp"
#include "node.hpp"
#include <iostream>
#include <utility>

red_black_tree::red_black_tree() {
    root = nullptr;
    byte_size = 0;
}

red_black_tree::~red_black_tree() {
   delete_tree();
}

void red_black_tree::delete_tree() {
    if (root != nullptr) {
        root->delete_tree();
    }

    root = nullptr;
    byte_size = 0;
}

void red_black_tree::insert(kv_pair new_pair) {
    rb_entry data = rb_entry{std::move(new_pair)};
    insert(data); 
}

void red_black_tree::insert(const rb_entry& data) {
    if (root == nullptr) {
        root = new node(data, true);
        byte_size = data.size();
    } else {
        uint64_t size_diff = root->insert(data);
        byte_size += size_diff;
    }
}

void red_black_tree::remove(const std::string& target) {
    if (root == nullptr) {
        return;
    }

    node* to_remove = root->find_node(rb_entry{target});
    if (to_remove == nullptr) {
        return;
    }

    bool tree_deleted = root->remove(to_remove);
    if (tree_deleted) {
        root = nullptr;
        byte_size = 0;
    } else {
        byte_size -= to_remove->pair.size();
    }
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
    if (root == nullptr) {
        return {};
    }

    node *x = root->floor(rb_entry{std::move(target)});
    if (x == nullptr) {
        return {};
    }
    return x->pair;
}

std::vector<rb_entry> red_black_tree::get_all_nodes() const {
    std::vector<rb_entry> nodes;
    root->in_order(nodes, false);
    return nodes;
}

std::vector<rb_entry> red_black_tree::get_and_delete_all_nodes() {
    std::vector<rb_entry> nodes;
    root->in_order(nodes, true);

    byte_size = 0;
    root = nullptr;

    return nodes;
}

uint64_t red_black_tree::size() const {
    return byte_size;
}

void red_black_tree::print() const {
    if (root != nullptr) {
        root->print_2d(0);
        std::cout << std::endl;
    }
}