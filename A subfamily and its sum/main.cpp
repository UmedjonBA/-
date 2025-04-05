#include <stdint.h>

#include <iostream>
#include <vector>

struct Node;

namespace AVLHelp {
int64_t GetHeight(Node* tree);
int64_t GetBalans(Node* tree);
int64_t GetSum(Node* tree);
void FixHeight(Node* tree);
void FixSum(Node* tree);
Node* RightRotate(Node* tree);
Node* LeftRotate(Node* tree);
Node* Balanced(Node* tree);
Node* FindMin(Node* tree);
Node* FindMax(Node* tree);
Node* EraseMin(Node* tree);
}  // namespace AVLHelp

struct Node {
  int64_t key;
  int64_t sum;
  int64_t height = 1;
  Node* left = nullptr;
  Node* right = nullptr;

  Node(int64_t key) : key(key), sum(key) {}

  void Clear() {
    if (left != nullptr) {
      left->Clear();
    }
    if (right != nullptr) {
      right->Clear();
    }

    delete this;
  }
};

int64_t AVLHelp::GetHeight(Node* tree) {
  return tree == nullptr ? 0 : tree->height;
}

int64_t AVLHelp::GetBalans(Node* tree) {
  if (tree == nullptr) {
    return 0;
  }
  return AVLHelp::GetHeight(tree->right) - AVLHelp::GetHeight(tree->left);
}

int64_t AVLHelp::GetSum(Node* tree) {
  if (tree == nullptr) {
    return 0;
  }

  return tree->sum;
}

void AVLHelp::FixHeight(Node* tree) {
  int64_t height_left = AVLHelp::GetHeight(tree->left);
  int64_t height_right = AVLHelp::GetHeight(tree->right);
  tree->height = std::max(height_left, height_right) + 1;
}

void AVLHelp::FixSum(Node* tree) {
  if (tree == nullptr) {
    return;
  }

  tree->sum =
      AVLHelp::GetSum(tree->left) + AVLHelp::GetSum(tree->right) + tree->key;
}

Node* AVLHelp::LeftRotate(Node* tree) {
  Node* new_tree_root = tree->right;

  auto new_root_sum = AVLHelp::GetSum(tree);
  auto tree_sum = AVLHelp::GetSum(tree->left) +
                  AVLHelp::GetSum(tree->right->left) + tree->key;

  tree->right = new_tree_root->left;
  new_tree_root->left = tree;

  new_tree_root->sum = new_root_sum;
  tree->sum = tree_sum;

  AVLHelp::FixHeight(tree);
  AVLHelp::FixHeight(new_tree_root);
  return new_tree_root;
}

Node* AVLHelp::RightRotate(Node* tree) {
  Node* new_tree_root = tree->left;

  auto new_root_sum = AVLHelp::GetSum(tree);
  auto tree_sum = AVLHelp::GetSum(tree->right) +
                  AVLHelp::GetSum(tree->left->right) + tree->key;

  tree->left = new_tree_root->right;
  new_tree_root->right = tree;

  new_tree_root->sum = new_root_sum;
  tree->sum = tree_sum;

  AVLHelp::FixHeight(tree);
  AVLHelp::FixHeight(new_tree_root);
  return new_tree_root;
}

Node* AVLHelp::Balanced(Node* tree) {
  if (AVLHelp::GetBalans(tree) == 2) {
    if (AVLHelp::GetBalans(tree->right) < 0) {
      tree->right = AVLHelp::RightRotate(tree->right);
    }
    return AVLHelp::LeftRotate(tree);
  }

  if (AVLHelp::GetBalans(tree) == -2) {
    if (AVLHelp::GetBalans(tree->left) > 0) {
      tree->left = AVLHelp::LeftRotate(tree->left);
    }
    return AVLHelp::RightRotate(tree);
  }

  return tree;
}

Node* AVLHelp::FindMin(Node* tree) {
  if (tree->left != nullptr) {
    return AVLHelp::FindMin(tree->left);
  }

  return tree;
}

Node* AVLHelp::FindMax(Node* tree) {
  if (tree->right != nullptr) {
    return AVLHelp::FindMax(tree->right);
  }

  return tree;
}

template <class Compare>
class AVLTree {
 private:
  Node* InsertBase(Node* tree, int64_t key) {
    if (tree == nullptr) {
      return new Node(key);
    }
    if (!cmp_(tree->key, key) && !cmp_(key, tree->key)) {
      return tree;
    }

    if (cmp_(key, tree->key)) {
      tree->left = InsertBase(tree->left, key);
      AVLHelp::FixSum(tree);
    } else {
      tree->right = InsertBase(tree->right, key);
      AVLHelp::FixSum(tree);
    }

    AVLHelp::FixHeight(tree);
    auto* return_node = AVLHelp::Balanced(tree);
    AVLHelp::FixSum(return_node);
    return return_node;
  }

  int64_t LessElemSumBase(Node* tree, int64_t key) const {
    if (tree == nullptr) {
      return 0;
    }

    if (!cmp_(tree->key, key) && !cmp_(key, tree->key)) {
      return AVLHelp::GetSum(tree->left) + tree->key;
    }

    if (cmp_(tree->key, key)) {
      if (tree->right == nullptr) {
        return tree->key + AVLHelp::GetSum(tree->left);
      }

      return LessElemSumBase(tree->right, key) + tree->key +
             AVLHelp::GetSum(tree->left);
    }

    if (tree->left == nullptr) {
      return 0;
    }

    return LessElemSumBase(tree->left, key);
  }

 public:
  AVLTree() = default;
  ~AVLTree() {
    if (tree_ != nullptr) {
      tree_->Clear();
    }
  }

  void Insert(int64_t key) { tree_ = InsertBase(tree_, key); }

  int64_t LessElemSum(int64_t left, int64_t right) const {
    if (tree_ == nullptr) {
      return 0;
    }
    if (left < 0) {
      return LessElemSumBase(tree_, right);
    }

    int64_t less_left = LessElemSumBase(tree_, left - 1);
    int64_t less_right = LessElemSumBase(tree_, right);
    return std::abs(less_right - less_left);
  }

 private:
  Compare cmp_ = Compare();
  Node* tree_ = nullptr;
};

struct Less {
  bool operator()(int64_t num1, int64_t num2) const { return num1 < num2; }
};

int main() {
  const int64_t kBigMod = 1000000000;

  uint64_t quantity_requests;
  uint64_t number_of_request = 0;
  std::cin >> quantity_requests;
  bool is_plus = true;
  uint64_t ans_prev_request = 0;

  AVLTree<Less> tree;

  while (number_of_request < quantity_requests) {
    char input;
    std::cin >> input;
    int64_t key;

    if (is_plus && (input == '+')) {
      std::cin >> key;
      tree.Insert(key);

    } else if (input == '?') {
      int64_t left;
      int64_t right;
      std::cin >> left >> right;

      is_plus = false;

      ans_prev_request = tree.LessElemSum(left, right);
      std::cout << ans_prev_request << '\n';

    } else if (!is_plus && (input == '+')) {
      std::cin >> key;
      int64_t real_key = (key + ans_prev_request) % kBigMod;
      is_plus = true;

      tree.Insert(real_key);
    }
    ++number_of_request;
  }
}