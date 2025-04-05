#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

namespace Jnal {
namespace DetailsAvl {
template <class Key>
struct Node {
  template <class... Args>
  explicit Node(Args&&... args)
      : key(std::forward<Args>(args)...), left(nullptr), right(nullptr) {}

  Key key;
  std::shared_ptr<Node<Key>> left;
  std::shared_ptr<Node<Key>> right;
};
}
}

template <typename T>
struct Node;

template <typename T, class Compare>
class AVLTreeBase;

template <typename T, class Compare>
class AVLTree;

template <typename T>
struct Node {
  T key;
  int64_t height = 1;
  std::shared_ptr<Node<T>> left = nullptr;
  std::shared_ptr<Node<T>> right = nullptr;

  explicit Node(const T& key) : key(key) {}
  explicit Node(T&& key) : key(std::move(key)) {}

  // Destructor is defaulted as shared_ptr handles memory management
  ~Node() = default;
};

template <typename T, class Compare>
class AVLTreeBase {
 public:
  // Destructor is defaulted as shared_ptr handles memory management
  ~AVLTreeBase() = default;

  int64_t GetHeight(const std::shared_ptr<Node<T>>& node) const;
  int64_t GetBalance(const std::shared_ptr<Node<T>>& node) const;
  void FixHeight(std::shared_ptr<Node<T>> node);
  std::shared_ptr<Node<T>> RightRotate(std::shared_ptr<Node<T>> node);
  std::shared_ptr<Node<T>> LeftRotate(std::shared_ptr<Node<T>> node);
  std::shared_ptr<Node<T>> Balance(std::shared_ptr<Node<T>> node);

  std::shared_ptr<Node<T>> FindMin(const std::shared_ptr<Node<T>>& node) const;
  std::shared_ptr<Node<T>> FindMax(const std::shared_ptr<Node<T>>& node) const;

  std::shared_ptr<Node<T>> Insert(std::shared_ptr<Node<T>> node, T&& key);
  std::shared_ptr<Node<T>> Insert(std::shared_ptr<Node<T>> node, const T& key);
  std::shared_ptr<Node<T>> Find(const std::shared_ptr<Node<T>>& node, const T& key) const;
  std::shared_ptr<Node<T>> Next(std::shared_ptr<Node<T>> node, std::shared_ptr<Node<T>> next, const T& key) const;

  Compare cmp = Compare();
  std::shared_ptr<Node<T>> root = nullptr;
};

template <typename T, class Compare>
int64_t AVLTreeBase<T, Compare>::GetHeight(const std::shared_ptr<Node<T>>& node) const {
  return node == nullptr ? 0 : node->height;
}

template <typename T, class Compare>
int64_t AVLTreeBase<T, Compare>::GetBalance(const std::shared_ptr<Node<T>>& node) const {
  if (node == nullptr) {
    return 0;
  }
  return GetHeight(node->right) - GetHeight(node->left);
}

template <typename T, class Compare>
void AVLTreeBase<T, Compare>::FixHeight(std::shared_ptr<Node<T>> node) {
  int64_t height_left = GetHeight(node->left);
  int64_t height_right = GetHeight(node->right);
  node->height = std::max(height_left, height_right) + 1;
}

template <typename T, class Compare>
std::shared_ptr<Node<T>> AVLTreeBase<T, Compare>::LeftRotate(std::shared_ptr<Node<T>> node) {
  std::shared_ptr<Node<T>> new_root = node->right;
  node->right = new_root->left;
  new_root->left = node;
  FixHeight(node);
  FixHeight(new_root);
  return new_root;
}

template <typename T, class Compare>
std::shared_ptr<Node<T>> AVLTreeBase<T, Compare>::RightRotate(std::shared_ptr<Node<T>> node) {
  std::shared_ptr<Node<T>> new_root = node->left;
  node->left = new_root->right;
  new_root->right = node;
  FixHeight(node);
  FixHeight(new_root);
  return new_root;
}

template <typename T, class Compare>
std::shared_ptr<Node<T>> AVLTreeBase<T, Compare>::Balance(std::shared_ptr<Node<T>> node) {
  if (GetBalance(node) == 2) {
    if (GetBalance(node->right) < 0) {
      node->right = RightRotate(node->right);
    }
    return LeftRotate(node);
  }

  if (GetBalance(node) == -2) {
    if (GetBalance(node->left) > 0) {
      node->left = LeftRotate(node->left);
    }
    return RightRotate(node);
  }

  return node;
}

template <typename T, class Compare>
std::shared_ptr<Node<T>> AVLTreeBase<T, Compare>::FindMin(const std::shared_ptr<Node<T>>& node) const {
  if (node->left != nullptr) {
    return FindMin(node->left);
  }
  return node;
}

template <typename T, class Compare>
std::shared_ptr<Node<T>> AVLTreeBase<T, Compare>::FindMax(const std::shared_ptr<Node<T>>& node) const {
  if (node->right != nullptr) {
    return FindMax(node->right);
  }
  return node;
}

template <typename T, class Compare>
std::shared_ptr<Node<T>> AVLTreeBase<T, Compare>::Insert(std::shared_ptr<Node<T>> node, T&& key) {
  if (node == nullptr) {
    return std::make_shared<Node<T>>(std::move(key));
  }
  if (!cmp(node->key, key) && !cmp(key, node->key)) {
    return node;
  }
  if (cmp(key, node->key)) {
    node->left = Insert(node->left, std::move(key));
  } else {
    node->right = Insert(node->right, std::move(key));
  }
  FixHeight(node);
  return Balance(node);
}

template <typename T, class Compare>
std::shared_ptr<Node<T>> AVLTreeBase<T, Compare>::Insert(std::shared_ptr<Node<T>> node, const T& key) {
  if (node == nullptr) {
    return std::make_shared<Node<T>>(key);
  }
  if (!cmp(node->key, key) && !cmp(key, node->key)) {
    return node;
  }
  if (cmp(key, node->key)) {
    node->left = Insert(node->left, key);
  } else {
    node->right = Insert(node->right, key);
  }
  FixHeight(node);
  return Balance(node);
}

template <typename T, class Compare>
std::shared_ptr<Node<T>> AVLTreeBase<T, Compare>::Find(const std::shared_ptr<Node<T>>& node, const T& key) const {
  if (node == nullptr) {
    return nullptr;
  }
  if (!cmp(node->key, key) && !cmp(key, node->key)) {
    return node;
  }
  if (cmp(key, node->key)) {
    if (node->left == nullptr) {
      return nullptr;
    }
    return Find(node->left, key);
  }
  if (node->right == nullptr) {
    return nullptr;
  }
  return Find(node->right, key);
}

template <typename T, class Compare>
std::shared_ptr<Node<T>> AVLTreeBase<T, Compare>::Next(std::shared_ptr<Node<T>> node, std::shared_ptr<Node<T>> next, const T& key) const {
  if (!cmp(node->key, key) && !cmp(key, node->key)) {
    return node;
  }
  if (cmp(key, node->key)) {
    if (node->left == nullptr) {
      return node;
    }
    return Next(node->left, node, key);
  }
  if (node->right == nullptr) {
    return next;
  }
  return Next(node->right, next, key);
}

template <typename T, class Compare>
class AVLTree : protected AVLTreeBase<T, Compare> {
 protected:
  using BaseTree = AVLTreeBase<T, Compare>;

 public:
  void Insert(T&& key) {
    BaseTree::root = BaseTree::Insert(BaseTree::root, std::move(key));
  }

  void Insert(const T& key) {
    BaseTree::root = BaseTree::Insert(BaseTree::root, key);
  }

  std::pair<T, bool> Find(const T& key) const {
    std::shared_ptr<Node<T>> result = BaseTree::Find(BaseTree::root, key);
    if (result == nullptr) {
      return std::make_pair(T(), false);
    }
    return std::make_pair(result->key, true);
  }

  std::pair<T, bool> Next(const T& key) const {
    if (BaseTree::root == nullptr) {
      return std::make_pair(T(), false);
    }
    std::shared_ptr<Node<T>> max = BaseTree::FindMax(BaseTree::root);
    if (BaseTree::cmp(max->key, key)) {
      return std::make_pair(T(), false);
    }
    std::shared_ptr<Node<T>> next_node = BaseTree::Next(BaseTree::root, nullptr, key);
    if (next_node != nullptr) {
      return std::make_pair(next_node->key, true);
    }
    return std::make_pair(T(), false);
  }
};

struct Comp {
  bool operator()(const uint32_t& lhs, const uint32_t& rhs) const {
    return lhs < rhs;
  }
};

struct InputData {
  int64_t num_operations;
  std::vector<std::pair<char, int64_t>> operations;
};

InputData GetInput() {
  InputData input;
  std::cin >> input.num_operations;
  input.operations.resize(static_cast<size_t>(input.num_operations));
  for (size_t i = 0; i < static_cast<size_t>(input.num_operations); ++i) {
    std::cin >> input.operations[i].first >> input.operations[i].second;
  }
  return input;
}

std::vector<int64_t> Solve(const InputData& input) {
  AVLTree<int64_t, Comp> tree;
  const int64_t kModulo = 1000000000;
  int64_t last_result = 0;
  std::vector<int64_t> results;

  for (size_t i = 0; i < input.operations.size(); ++i) {
    char operation = input.operations[i].first;
    int64_t value = input.operations[i].second;
    if (operation == '+') {
      tree.Insert((value + last_result) % kModulo);
      last_result = 0;
    } else {
      auto result = tree.Next(value);
      if (result.second) {
        last_result = result.first;
        results.push_back(last_result);
      } else {
        results.push_back(-1);
        last_result = -1;
      }
    }
  }
  return results;
}

void PrintOutput(const std::vector<int64_t>& results) {
  for (size_t i = 0; i < results.size(); ++i) {
    std::cout << results[i] << '\n';
  }
}

int main() {
  InputData input = GetInput();
  std::vector<int64_t> results = Solve(input);
  PrintOutput(results);
  return 0;
}
