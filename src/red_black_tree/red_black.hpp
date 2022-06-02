#ifndef RED_BLACK_H 
#define RED_BLACK_H

#include "data.hpp"
#include "node.hpp"
#include "../utils/types.hpp"
#include <cstdint>
#include <vector>
#include <optional>

class red_black_tree {

    public:
        uint64_t size;

        red_black_tree();

        ~red_black_tree();

        void delete_tree();

        void insert(kv_pair new_pair);

        void remove(const std::string& target);

        bool exists(std::string target) const;

        std::optional<std::string> get(std::string target) const;

        std::optional<rb_entry> floor(std::string target) const;

        std::vector<rb_entry> get_all_nodes() const;

        std::vector<rb_entry> get_and_delete_all_nodes();

        void print() const;

    private:
        node* root;

        void insert(const rb_entry& data);
};

#endif // RED_BLACK_H