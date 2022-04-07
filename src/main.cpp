#include "bloom_filter/bloom.hpp"
#include <iostream>

int main() {
    bloom_filter bloom(100);

    bloom.set(10);
    std::cout << bloom.is_set(10) << std::endl;

    return 0;
}
