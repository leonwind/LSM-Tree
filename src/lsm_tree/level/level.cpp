#include "level.hpp"

level::level(size_t max_size){
    this->max_size = max_size;
}

void level::remove(){}

level level::merge(const level& other){
    return other;
}
