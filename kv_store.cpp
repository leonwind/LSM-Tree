#include "src/lsm_tree/lsm_tree.hpp"

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
    command_loop();

    return 0;
}
