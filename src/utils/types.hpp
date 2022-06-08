#ifndef TYPES_H
#define TYPES_H

#include <string>

// Bad solution since this value is not allowed to be used by the user
#define TOMBSTONE "+++TOMBSTONE+++"
#define SEPERATOR ','

struct kv_pair {
    std::string key;
    std::string val;

    bool empty() const {
        return key.empty() and val.empty();
    }

    std::string to_log_entry() const {
        return key + SEPERATOR + val + "\n";
    }

    static kv_pair split_log_entry(const std::string& line) {
        if (line.empty()) {
            return {"", ""};
        }

        size_t seperator_pos = line.find(SEPERATOR);
        std::string key = line.substr(0, seperator_pos);
        std::string value = line.substr(seperator_pos + 1, line.size() - seperator_pos);
        return {key, value};
    }

    std::strong_ordering operator <=>(const kv_pair& other) const {
        if (key == other.key) {
            return std::strong_ordering::equal;
        }

        if (key < other.key) {
            return std::strong_ordering::less;
        }

        return std::strong_ordering::greater;
    }

    bool operator==(const kv_pair& other) const {return key == other.key;}
};

#endif // TYPES_H