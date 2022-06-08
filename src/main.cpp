#include "lsm_tree/lsm_tree.hpp"
#include "bloom_filter/bloom.hpp"
#include "red_black_tree/red_black.hpp"
#include "utils/types.hpp"
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

    rb_tree.insert(kv_pair{"8", ""});
    rb_tree.insert(kv_pair{"18", ""});
    rb_tree.insert(kv_pair{"5", ""});
    rb_tree.insert(kv_pair{"15", ""});
    rb_tree.insert(kv_pair{"17", ""});
    rb_tree.insert(kv_pair{"25", ""});
    rb_tree.insert(kv_pair{"40", ""});
    rb_tree.insert(kv_pair{"80", ""});
    rb_tree.insert(kv_pair{"19", ""});
    rb_tree.remove("25");
    rb_tree.remove("8");
    rb_tree.remove("17");
    rb_tree.print();
}

enum commands {
    PUT,
    GET,
    DELETE,
    DROP,
    EXIT,
    NONE,
};

commands cmd_to_enum(const std::string& in_str) {
    if (in_str == "PUT" || in_str == "put") return PUT;
    if (in_str == "GET" || in_str == "get") return GET;
    if (in_str == "DELETE" || in_str == "delete") return DELETE;
    if (in_str == "DROP" || in_str == "drop") return DROP;
    if (in_str == "EXIT" || in_str == "exit") return EXIT;
    return NONE;
}

void command_loop() {
    lsm_tree db;
    bool run = true;

    std::string command;
    std::string key, value;

    while (run) {
        std::cout << "> ";
        std::cin >> command;

        switch(cmd_to_enum(command)) {
            case PUT:
                std::cin >> key >> value;
                if (key.empty()) {
                    std::cout << "Key can not be empty." << std::endl;
                } else if (value.empty()) {
                    std::cout << "Value can not be empty." << std::endl;
                } else {
                    db.put(key, value);
                }
                break;

            case GET:
                std::cin >> key;
                value = db.get(key);
                std::cout << value << std::endl;
                break;

            case DELETE:
                std::cin >> key;
                db.remove(key);
                break;

            case DROP:
                db.drop_table();
                break;

            case EXIT:
                run = false;
                break;

            default:
                std::cout << "Command '" << command << "' not found." << std::endl; 
                break;
        } 
    }
}

int main() {
    //test_red_black_tree();
    command_loop();

    return 0;
}
