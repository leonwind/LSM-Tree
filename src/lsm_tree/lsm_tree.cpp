#include "lsm_tree.hpp"

#include <iostream>

lsm_tree::lsm_tree(): bloom(BLOOM_SIZE), rb_tree() {
}

lsm_tree::~lsm_tree() {
}

void lsm_tree::put(std::string key, std::string value) {
    rb_tree.insert({key, value});
}

std::string lsm_tree::get(std::string key) {
    return rb_tree.get(key);
}

void lsm_tree::remove(std::string key) {
    rb_tree.remove(key);
}

std::vector<entry> lsm_tree::range(std::string start, size_t len) {
    std::vector<entry> pairs;
    return pairs;
}
