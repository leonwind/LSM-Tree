#include "bloom_filter/bloom.hpp"
#include <iostream>

int main() {
    bloom_filter bloom(100);

    bloom.set("test");
    std::cout << bloom.is_set("test") << std::endl;
    std::cout << bloom.is_set("non test") << std::endl;

    bloom.set("non test");
    std::cout << bloom.is_set("non test") << std::endl;

    return 0;
}
