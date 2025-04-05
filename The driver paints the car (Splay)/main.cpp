#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct Node {
    std::string key;
    std::string value;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
    std::weak_ptr<Node> parent;

    Node(const std::string& key_val, const std::string& val)
        : key(key_val),
          value(val),
          left(nullptr),
          right(nullptr),
          parent() {}
};

class SplayTree {
private:
    std::shared_ptr<Node> root_;

    void Rotate(std::shared_ptr<Node> current_node) {
        std::shared_ptr<Node> parent_node = current_node->parent.lock();
        std::shared_ptr<Node> grand_node = parent_node ? parent_node->parent.lock() : nullptr;
        if (grand_node) {
            if (grand_node->left == parent_node) {
                grand_node->left = current_node;
            } else {
                grand_node->right = current_node;
            }
            current_node->parent = grand_node;
        } else {
            current_node->parent.reset();
            root_ = current_node;
        }
        if (current_node == parent_node->left) {
            parent_node->left = current_node->right;
            if (current_node->right) {
                current_node->right->parent = parent_node;
            }
            current_node->right = parent_node;
        } else {
            parent_node->right = current_node->left;
            if (current_node->left) {
                current_node->left->parent = parent_node;
            }
            current_node->left = parent_node;
        }
        parent_node->parent = current_node;
    }

    void Splay(std::shared_ptr<Node> current_node) {
        while (auto parent_node = current_node->parent.lock()) {
            auto grand_node = parent_node->parent.lock();
            if (!grand_node) {
                Rotate(current_node);
            } else if ((grand_node->left == parent_node && parent_node->left == current_node) ||
                       (grand_node->right == parent_node && parent_node->right == current_node)) {
                Rotate(parent_node);
                Rotate(current_node);
            } else {
                Rotate(current_node);
                Rotate(current_node);
            }
        }
    }

    std::shared_ptr<Node> Find(const std::string& search_key) {
        std::shared_ptr<Node> current_node = root_;
        while (current_node) {
            if (search_key < current_node->key) {
                current_node = current_node->left;
            } else if (search_key > current_node->key) {
                current_node = current_node->right;
            } else {
                Splay(current_node);
                return current_node;
            }
        }
        return nullptr;
    }

    void Insert(const std::string& insert_key, const std::string& insert_value) {
        if (!root_) {
            root_ = std::make_shared<Node>(insert_key, insert_value);
            return;
        }
        std::shared_ptr<Node> current_node = root_;
        while (true) {
            if (insert_key < current_node->key) {
                if (!current_node->left) {
                    current_node->left = std::make_shared<Node>(insert_key, insert_value);
                    current_node->left->parent = current_node;
                    Splay(current_node->left);
                    return;
                }
                current_node = current_node->left;
            } else if (insert_key > current_node->key) {
                if (!current_node->right) {
                    current_node->right = std::make_shared<Node>(insert_key, insert_value);
                    current_node->right->parent = current_node;
                    Splay(current_node->right);
                    return;
                }
                current_node = current_node->right;
            } else {
                Splay(current_node);
                return;
            }
        }
    }

public:
    SplayTree() : root_(nullptr) {}
    ~SplayTree() = default;

    void Add(const std::string& add_key, const std::string& add_value) {
        Insert(add_key, add_value);
    }

    std::string Get(const std::string& get_key) {
        std::shared_ptr<Node> result_node = Find(get_key);
        if (result_node) {
            return result_node->value;
        }
        return "";
    }
};

struct InputData {
    int n;
    std::vector<std::pair<std::string, std::string>> entries;
    int q;
    std::vector<std::string> queries;
};

void ReadInput(InputData& input_data) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cin >> input_data.n;
    input_data.entries.resize(static_cast<size_t>(input_data.n));
    for (size_t index = 0; index < static_cast<size_t>(input_data.n); ++index) {
        std::cin >> input_data.entries[index].first >> input_data.entries[index].second;
    }
    std::cin >> input_data.q;
    input_data.queries.resize(static_cast<size_t>(input_data.q));
    for (size_t index = 0; index < static_cast<size_t>(input_data.q); ++index) {
        std::cin >> input_data.queries[index];
    }
}

std::vector<std::string> Solve(const InputData& input_data) {
    SplayTree calls;
    SplayTree cars;
    for (size_t index = 0; index < static_cast<size_t>(input_data.n); ++index) {
        calls.Add(input_data.entries[index].first, input_data.entries[index].second);
        cars.Add(input_data.entries[index].second, input_data.entries[index].first);
    }
    std::vector<std::string> result;
    result.reserve(static_cast<size_t>(input_data.q));
    for (size_t index = 0; index < static_cast<size_t>(input_data.q); ++index) {
        const std::string& query_val = input_data.queries[index];
        std::string tmp_val = calls.Get(query_val);
        if (tmp_val.empty()) {
            result.push_back(cars.Get(query_val));
        } else {
            result.push_back(tmp_val);
        }
    }
    return result;
}

void PrintOutput(const std::vector<std::string>& result) {
    for (const auto& result_val : result) {
        std::cout << result_val << "\n";
    }
}

int main() {
    InputData input_data;
    ReadInput(input_data);
    auto res = Solve(input_data);
    PrintOutput(res);
    return 0;
}
