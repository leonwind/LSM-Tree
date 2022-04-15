#include "lsm_tree.hpp"

lsm_tree::lsm_tree() {
    bloom_filter bloom(BLOOM_SIZE);
    red_black_tree rb_tree;
}

lsm_tree::~lsm_tree() {
    delete bloom;
    delete rb_tree;
}

void lsm_tree::put(std::string key, std::string value) {
    rb_tree->insert({key, value});
}

std::string lsm_tree::get(std::string key) {
    return rb_tree->get(key);
}

void lsm_tree::remove(std::string key) {
    rb_tree->remove(key);
}

std::vector<entry> lsm_tree::range(std::string start, size_t len) {
    std::vector<entry> pairs;
    return pairs;
}
