#include "bloom_filter/bloom.hpp"
#include "red_black_tree/red_black.hpp"
#include "types.hpp"
#include <iostream>

void test_bloom_filter() {
    bloom_filter bloom(100);

    bloom.set("test");
    std::cout << bloom.is_set("test") << std::endl;
    std::cout << bloom.is_set("non test") << std::endl;

    bloom.set("non test");
    std::cout << bloom.is_set("non test") << std::endl;

}

void test_red_black_tree() {
    red_black_tree rb_tree;

    /*
    rb_tree.insert({"1", ""});
    rb_tree.insert({"2", ""});
    rb_tree.insert({"3", ""});
    rb_tree.insert({"4", ""});
    rb_tree.insert({"4", ""});
    rb_tree.insert({"5", ""});
    rb_tree.insert({"6", ""});
    rb_tree.insert({"7", ""});
    rb_tree.insert({"8", ""});
    rb_tree.insert({"9", ""});
    rb_tree.print();

    std::cout << "------" << std::endl;

    rb_tree.remove("8");
    rb_tree.print();
    */

    rb_tree.insert({"8", ""});
    rb_tree.insert({"18", ""});
    rb_tree.insert({"5", ""});
    //std::cout << "----------" << std::endl;
    rb_tree.insert({"15", ""});
    //rb_tree.print();
    rb_tree.insert({"17", ""});
    rb_tree.insert({"25", ""});
    rb_tree.insert({"40", ""});
    //std::cout << "----------" << std::endl;
    rb_tree.insert({"80", ""});
    rb_tree.insert({"19", ""});
    rb_tree.remove("25");
    rb_tree.remove("8");
    rb_tree.remove("17");
    rb_tree.print();
    
    //std::cout << "----------" << std::endl;
    //rb_tree.remove("8");
}

int main() {
    test_red_black_tree();

    return 0;
}
