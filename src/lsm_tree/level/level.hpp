#ifndef LEVEL_H
#define LEVEL_H

#include <string>

class level {

    public:
        level(size_t max_size);

        void remove();
        static level merge(const level& other);

    private:
        std::string path;
        size_t max_size;
        size_t curr_size;
};

#endif // LEVEL_H
