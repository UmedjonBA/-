#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <limits>
#include <algorithm>

struct InputData {
    int n, m;
    std::vector<long long> arr;
    struct Query {
        int op;
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
    bool rev;
    std::shared_ptr<TreapNode> left;
    std::shared_ptr<TreapNode> right;
    TreapNode(const T &val) : value(val), subtree_min(val), size(1), priority(gen()), rev(false), left(nullptr), right(nullptr) {}
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

    static void push(std::shared_ptr<TreapNode<T>> &t) {
        if (t && t->rev) {
            std::swap(t->left, t->right);
            if (t->left) t->left->rev = !t->left->rev;
            if (t->right) t->right->rev = !t->right->rev;
            t->rev = false;
        }
    }

    static void update(std::shared_ptr<TreapNode<T>> &t) {
        if (!t) return;
        t->size = 1 + get_size(t->left) + get_size(t->right);
        push(t->left);
        push(t->right);
        T left_min = get_min_val(t->left);
        T right_min = get_min_val(t->right);
        t->subtree_min = std::min(t->value, std::min(left_min, right_min));
    }

    static void split(std::shared_ptr<TreapNode<T>> t, int key, std::shared_ptr<TreapNode<T>> &l, std::shared_ptr<TreapNode<T>> &r) {
        if (!t) {
            l = r = nullptr;
            return;
        }
        push(t);
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
        push(l);
        push(r);
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

    void build(const std::vector<T> &arr) {
        for (auto &x : arr) {
            std::shared_ptr<TreapNode<T>> new_node = std::make_shared<TreapNode<T>>(x);
            root = merge(root, new_node);
        }
    }

    void reverse_segment(int l, int r) {
        std::shared_ptr<TreapNode<T>> A = nullptr;
        std::shared_ptr<TreapNode<T>> B = nullptr;
        std::shared_ptr<TreapNode<T>> C = nullptr;
        split(root, l-1, A, B);
        split(B, r-l+1, B, C);
        if (B) B->rev = !B->rev;
        root = merge(A, merge(B, C));
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
    std::cin >> data.n >> data.m;
    data.arr.resize(data.n);
    for (int i = 0; i < data.n; i++) {
        std::cin >> data.arr[i];
    }
    data.queries.resize(data.m);
    for (int i = 0; i < data.m; i++) {
        std::cin >> data.queries[i].op >> data.queries[i].l >> data.queries[i].r;
    }
}

std::vector<long long> solve(const InputData &data) {
    Treap<long long> treap;
    treap.build(data.arr);
    std::vector<long long> answers;
    for (auto &q : data.queries) {
        if (q.op == 1) {
            treap.reverse_segment(q.l, q.r);
        } else {
            answers.push_back(treap.query_min(q.l, q.r));
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
