#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <limits>
#include <algorithm>

struct InputData {
    int n;
    struct Query {
        char op;
        int i;
        long long x;
        int l, r;
    };
    std::vector<Query> queries;
};

struct RandomGenerator {
    std::mt19937_64 rng;
    RandomGenerator() {
        std::random_device rd;
        rng.seed(rd());
    }
    uint64_t operator()() {
        return rng();
    }
};

RandomGenerator gen;

template<typename T>
struct TreapNode {
    T value;
    T subtree_min;
    int size;
    uint64_t priority;
    std::shared_ptr<TreapNode> left;
    std::shared_ptr<TreapNode> right;
    TreapNode(const T &val) : value(val), subtree_min(val), size(1), priority(gen()), left(nullptr), right(nullptr) {}
};

template<typename T>
struct Treap {
    std::shared_ptr<TreapNode<T>> root = nullptr;

    static int get_size(const std::shared_ptr<TreapNode<T>> &t) {
        return t ? t->size : 0;
    }

    static T get_min_val(const std::shared_ptr<TreapNode<T>> &t) {
        if (!t) return std::numeric_limits<T>::max();
        return t->subtree_min;
    }

    static void update(std::shared_ptr<TreapNode<T>> &t) {
        if (!t) return;
        t->size = 1 + get_size(t->left) + get_size(t->right);
        T left_min = get_min_val(t->left);
        T right_min = get_min_val(t->right);
        t->subtree_min = std::min({t->value, left_min, right_min});
    }

    static void split(std::shared_ptr<TreapNode<T>> t, int key, std::shared_ptr<TreapNode<T>> &l, std::shared_ptr<TreapNode<T>> &r) {
        if (!t) {
            l = r = nullptr;
            return;
        }
        int cur_size = get_size(t->left) + 1;
        if (key < cur_size) {
            split(t->left, key, l, t->left);
            r = t;
            update(r);
        } else {
            split(t->right, key - cur_size, t->right, r);
            l = t;
            update(l);
        }
    }

    static std::shared_ptr<TreapNode<T>> merge(std::shared_ptr<TreapNode<T>> l, std::shared_ptr<TreapNode<T>> r) {
        if (!l) return r;
        if (!r) return l;
        if (l->priority > r->priority) {
            l->right = merge(l->right, r);
            update(l);
            return l;
        } else {
            r->left = merge(l, r->left);
            update(r);
            return r;
        }
    }

    void insert(int pos, const T &val) {
        std::shared_ptr<TreapNode<T>> new_node = std::make_shared<TreapNode<T>>(val);
        std::shared_ptr<TreapNode<T>> L = nullptr;
        std::shared_ptr<TreapNode<T>> R = nullptr;
        split(root, pos, L, R);
        root = merge(merge(L, new_node), R);
    }

    T query_min(int l, int r) {
        std::shared_ptr<TreapNode<T>> A = nullptr;
        std::shared_ptr<TreapNode<T>> B = nullptr;
        std::shared_ptr<TreapNode<T>> C = nullptr;
        split(root, l-1, A, B);
        split(B, r-l+1, B, C);
        T ans = get_min_val(B);
        root = merge(A, merge(B, C));
        return ans;
    }
};

void input_data(InputData &data) {
    std::cin >> data.n;
    data.queries.resize(data.n);
    for (int i = 0; i < data.n; i++) {
        std::cin >> data.queries[i].op;
        if (data.queries[i].op == '+') {
            std::cin >> data.queries[i].i >> data.queries[i].x;
        } else {
            std::cin >> data.queries[i].l >> data.queries[i].r;
        }
    }
}

std::vector<long long> solve(const InputData &data) {
    Treap<long long> treap;
    std::vector<long long> answers;
    for (int i = 0; i < data.n; i++) {
        if (data.queries[i].op == '+') {
            treap.insert(data.queries[i].i, data.queries[i].x);
        } else {
            answers.push_back(treap.query_min(data.queries[i].l, data.queries[i].r));
        }
    }
    return answers;
}

void output_data(const std::vector<long long> &answers) {
    for (auto &ans : answers) {
        std::cout << ans << "\n";
    }
}

int main() {
    InputData data;
    input_data(data);
    auto answers = solve(data);
    output_data(answers);
    return 0;
}
