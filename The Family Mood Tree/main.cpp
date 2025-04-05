#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
#include <sstream>
#include <memory>

template <typename Key>
struct Node {
  bool is_leaf = false;
  size_t subtree_size = 0;
  std::deque<Key> keys;
  std::deque<std::shared_ptr<Node<Key>>> childs;
};

template <typename Key, size_t param, class Compare = std::less<Key>>
class BTreeBase {
 private:
  void SupportKeys(std::shared_ptr<Node<Key>> child, 
                   std::shared_ptr<Node<Key>> left, 
                   std::shared_ptr<Node<Key>> right);
  void SupportChilds(std::shared_ptr<Node<Key>> child, 
                     std::shared_ptr<Node<Key>> left, 
                     std::shared_ptr<Node<Key>> right);
  void SupportSize(std::shared_ptr<Node<Key>> left, 
                   std::shared_ptr<Node<Key>> right);

  void ShiftToLeftKey(std::shared_ptr<Node<Key>> node, size_t pos);
  void ShiftToRightKey(std::shared_ptr<Node<Key>> node, size_t pos);

 protected:
  std::shared_ptr<Node<Key>> Merge(std::shared_ptr<Node<Key>> node, size_t pos);

  size_t GetSizeTree(std::shared_ptr<Node<Key>> node) const;

  void SplitRoot();
  void SplitChild(std::shared_ptr<Node<Key>> node, size_t pos);

  void InsertBase(std::shared_ptr<Node<Key>> node, const Key& key);
  void InsertInLeaf(std::shared_ptr<Node<Key>> node, const Key& key);
  void InsertBase(std::shared_ptr<Node<Key>> node, Key&& key);
  void InsertInLeaf(std::shared_ptr<Node<Key>> node, Key&& key);

  void EraseBase(std::shared_ptr<Node<Key>> node, const Key& key);
  void EraseInLeaf(std::shared_ptr<Node<Key>> node, const Key& key);
  void SupportInvariant(std::shared_ptr<Node<Key>> node, size_t pos);
  void EraseInVertex(std::shared_ptr<Node<Key>> node, size_t pos, const Key& key);

  bool Exists(std::shared_ptr<Node<Key>> node, const Key& key) const;
  std::pair<Key, bool> Next(std::shared_ptr<Node<Key>> node, const Key& next, const Key& key) const;
  std::pair<Key, bool> Prev(std::shared_ptr<Node<Key>> node, const Key& prev, const Key& key) const;
  std::pair<Key, bool> KthStatistics(std::shared_ptr<Node<Key>> node, size_t number) const;

 public:
  ~BTreeBase() = default;

  Key& GetMax(std::shared_ptr<Node<Key>> node);
  Key& GetMin(std::shared_ptr<Node<Key>> node);
  const Key& GetMax(std::shared_ptr<Node<Key>> node) const;
  const Key& GetMin(std::shared_ptr<Node<Key>> node) const;

 protected:
  Compare cmp_ = Compare();
  std::shared_ptr<Node<Key>> root_ = nullptr;
};

template <typename Key, size_t param, class Compare = std::less<Key>>
class BTree : public BTreeBase<Key, param, Compare> {
 protected:
  using BaseTree = BTreeBase<Key, param, Compare>;

 public:
  void Insert(Key&& key);
  void Insert(const Key& key);
  void Erase(const Key& key);

  bool Exists(const Key& key) const;
  size_t Size() const;

  std::pair<Key, bool> Next(const Key& key) const;
  std::pair<Key, bool> Prev(const Key& key) const;
  std::pair<Key, bool> KthStatistics(size_t number) const;
};

template <typename Key, size_t param, class Compare>
size_t BTreeBase<Key, param, Compare>::GetSizeTree(std::shared_ptr<Node<Key>> node) const {
  if (!node) {
    return 0;
  }
  if (!root_) {
    return 0;
  }
  return node->subtree_size;
}

template <typename Key, size_t param, class Compare>
const Key& BTreeBase<Key, param, Compare>::GetMax(std::shared_ptr<Node<Key>> node) const {
  if (node->is_leaf) {
    return node->keys.back();
  }
  return GetMax(node->childs.back());
}
template <typename Key, size_t param, class Compare>
const Key& BTreeBase<Key, param, Compare>::GetMin(std::shared_ptr<Node<Key>> node) const {
  if (node->is_leaf) {
    return node->keys.front();
  }
  return GetMin(node->childs.front());
}

template <typename Key, size_t param, class Compare>
Key& BTreeBase<Key, param, Compare>::GetMax(std::shared_ptr<Node<Key>> node) {
  if (node->is_leaf) {
    return node->keys.back();
  }
  return GetMax(node->childs.back());
}
template <typename Key, size_t param, class Compare>
Key& BTreeBase<Key, param, Compare>::GetMin(std::shared_ptr<Node<Key>> node) {
  if (node->is_leaf) {
    return node->keys.front();
  }
  return GetMin(node->childs.front());
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::SupportKeys(std::shared_ptr<Node<Key>> child,
                                                 std::shared_ptr<Node<Key>> left,
                                                 std::shared_ptr<Node<Key>> right) {
  for (size_t i = 0; i < param - 1; ++i) {
    left->keys.push_back(std::move(child->keys[i]));
  }
  for (size_t i = param; i < 2 * param - 1; ++i) {
    right->keys.push_back(std::move(child->keys[i]));
  }
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::SupportChilds(std::shared_ptr<Node<Key>> child,
                                                   std::shared_ptr<Node<Key>> left,
                                                   std::shared_ptr<Node<Key>> right) {
  for (size_t i = 0; i < param; ++i) {
    left->childs.push_back(child->childs[i]);
    child->childs[i].reset();
  }
  for (size_t i = param; i < 2 * param; ++i) {
    right->childs.push_back(child->childs[i]);
    child->childs[i].reset();
  }
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::SupportSize(std::shared_ptr<Node<Key>> left,
                                                 std::shared_ptr<Node<Key>> right) {
  for (const auto& child : left->childs) {
    left->subtree_size += GetSizeTree(child);
  }
  left->subtree_size += param - 1;

  for (const auto& child : right->childs) {
    right->subtree_size += GetSizeTree(child);
  }
  right->subtree_size += param - 1;
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::InsertBase(std::shared_ptr<Node<Key>> node, Key&& key) {
  if (node->is_leaf) {
    InsertInLeaf(node, std::move(key));
    return;
  }

  auto iter_key =
      std::lower_bound(node->keys.begin(), node->keys.end(), key, cmp_);
  size_t pos = static_cast<size_t>(std::distance(node->keys.begin(), iter_key));
  auto iter_child = node->childs.begin() + pos;

  if ((*iter_child)->keys.size() == 2 * param - 1) {
    SplitChild(node, pos);
    InsertBase(node, std::move(key));
    return;
  }

  InsertBase(*iter_child, std::move(key));
  ++node->subtree_size;
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::InsertInLeaf(std::shared_ptr<Node<Key>> node, Key&& key) {
  auto iter_key =
      std::lower_bound(node->keys.begin(), node->keys.end(), key, cmp_);
  node->keys.insert(iter_key, std::move(key));
  ++node->subtree_size;
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::InsertBase(std::shared_ptr<Node<Key>> node,
                                                const Key& key) {
  if (node->is_leaf) {
    InsertInLeaf(node, key);
    return;
  }

  auto iter_key =
      std::lower_bound(node->keys.begin(), node->keys.end(), key, cmp_);
  size_t pos = static_cast<size_t>(std::distance(node->keys.begin(), iter_key));
  auto iter_child = node->childs.begin() + pos;

  if ((*iter_child)->keys.size() == 2 * param - 1) {
    SplitChild(node, pos);
    InsertBase(node, key);
    return;
  }

  InsertBase(*iter_child, key);
  ++node->subtree_size;
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::InsertInLeaf(std::shared_ptr<Node<Key>> node,
                                                  const Key& key) {
  auto iter_key =
      std::lower_bound(node->keys.begin(), node->keys.end(), key, cmp_);
  node->keys.insert(iter_key, key);
  ++node->subtree_size;
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::SplitRoot() {
  auto new_root = std::make_shared<Node<Key>>();
  auto left = std::make_shared<Node<Key>>();
  auto right = std::make_shared<Node<Key>>();

  if (!root_->is_leaf) {
    SupportChilds(root_, left, right);
  } else {
    left->is_leaf = true;
    right->is_leaf = true;
  }

  SupportKeys(root_, left, right);
  new_root->keys.push_back(std::move(root_->keys[param - 1]));
  new_root->childs = {left, right};

  SupportSize(left, right);
  new_root->subtree_size = root_->subtree_size;

  root_ = new_root;
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::SplitChild(std::shared_ptr<Node<Key>> node, size_t pos) {
  auto child = node->childs[pos];
  auto left = std::make_shared<Node<Key>>();
  auto right = std::make_shared<Node<Key>>();

  if (!child->is_leaf) {
    SupportChilds(child, left, right);
  } else {
    left->is_leaf = true;
    right->is_leaf = true;
  }

  SupportKeys(child, left, right);

  auto place_to_insert_key = node->keys.begin() + static_cast<int>(pos);
  node->keys.insert(place_to_insert_key, std::move(child->keys[param - 1]));
  auto place_to_insert_child = node->childs.begin() + static_cast<int>(pos + 1);
  node->childs[pos] = left;
  node->childs.insert(place_to_insert_child, right);

  SupportSize(left, right);
  // child shared_ptr перестанет быть использованным - освободится автоматически
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::ShiftToLeftKey(std::shared_ptr<Node<Key>> node,
                                                    size_t pos) {
  auto dest_child = node->childs[pos];
  auto scr_child = node->childs[pos + 1];

  if (!scr_child->is_leaf) {
    scr_child->subtree_size -= GetSizeTree(scr_child->childs.front());
    dest_child->subtree_size += GetSizeTree(scr_child->childs.front());
  }
  --scr_child->subtree_size;
  ++dest_child->subtree_size;

  if (!scr_child->is_leaf) {
    dest_child->childs.push_back(scr_child->childs.front());
    scr_child->childs.pop_front();
  }

  dest_child->keys.push_back(std::move(node->keys[pos]));
  node->keys[pos] = std::move(scr_child->keys.front());
  scr_child->keys.pop_front();
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::ShiftToRightKey(std::shared_ptr<Node<Key>> node,
                                                     size_t pos) {
  auto dest_child = node->childs[pos];
  auto scr_child = node->childs[pos - 1];

  if (!scr_child->is_leaf) {
    scr_child->subtree_size -= GetSizeTree(scr_child->childs.back());
    dest_child->subtree_size += GetSizeTree(scr_child->childs.back());
  }
  --scr_child->subtree_size;
  ++dest_child->subtree_size;

  if (!scr_child->is_leaf) {
    dest_child->childs.push_front(scr_child->childs.back());
    scr_child->childs.pop_back();
  }

  dest_child->keys.push_front(std::move(node->keys[pos - 1]));
  node->keys[pos - 1] = std::move(scr_child->keys.back());
  scr_child->keys.pop_back();
}

template <typename Key, size_t param, class Compare>
std::shared_ptr<Node<Key>> BTreeBase<Key, param, Compare>::Merge(std::shared_ptr<Node<Key>> node, size_t pos) {
  auto left_child = node->childs[pos];
  auto right_child = node->childs[pos + 1];

  auto new_child = std::make_shared<Node<Key>>();

  new_child->is_leaf = left_child->is_leaf;
  new_child->subtree_size =
      left_child->subtree_size + right_child->subtree_size;
  ++new_child->subtree_size;

  if (!left_child->is_leaf) {
    new_child->childs.resize(2 * param);
    for (size_t i = 0; i < param; ++i) {
      new_child->childs[i] = left_child->childs[i];
    }
    for (size_t i = 0; i < param; ++i) {
      new_child->childs[i + param] = right_child->childs[i];
    }
  }

  new_child->keys.resize(2 * param - 1);
  for (size_t i = 0; i < param - 1; ++i) {
    new_child->keys[i] = std::move(left_child->keys[i]);
  }
  new_child->keys[param - 1] = std::move(node->keys[pos]);
  for (size_t i = 0; i < param - 1; ++i) {
    new_child->keys[i + param] = std::move(right_child->keys[i]);
  }

  auto iter_child = node->childs.begin() + static_cast<int>(pos);
  node->childs.erase(iter_child);
  node->childs[pos] = new_child;

  auto iter_key = node->keys.begin() + static_cast<int>(pos);
  node->keys.erase(iter_key);

  // left_child и right_child были в node->childs, ссылки удалены. 
  // При отсутствии других ссылок на них они будут уничтожены автоматически.

  return new_child;
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::EraseBase(std::shared_ptr<Node<Key>> node,
                                               const Key& key) {
  if (node->is_leaf) {
    EraseInLeaf(node, key);
    return;
  }

  auto iter_key =
      std::lower_bound(node->keys.begin(), node->keys.end(), key, cmp_);

  size_t pos = static_cast<size_t>(std::distance(node->keys.begin(), iter_key));
  auto child = node->childs[pos];

  if (iter_key != node->keys.end() && *iter_key == key) {
    EraseInVertex(node, pos, key);
    return;
  }

  if (child->keys.size() == param - 1) {
    SupportInvariant(node, pos);
    EraseBase(node, key);
    return;
  }

  EraseBase(child, key);
  --node->subtree_size;
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::EraseInLeaf(std::shared_ptr<Node<Key>> node,
                                                 const Key& key) {
  auto iter_key =
      std::lower_bound(node->keys.begin(), node->keys.end(), key, cmp_);
  node->keys.erase(iter_key);
  --node->subtree_size;
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::SupportInvariant(std::shared_ptr<Node<Key>> node,
                                                      size_t pos) {
  std::shared_ptr<Node<Key>> left_brother = pos == 0 ? nullptr : node->childs[pos - 1];
  std::shared_ptr<Node<Key>> right_brother =
      pos == node->keys.size() ? nullptr : node->childs[pos + 1];

  if (left_brother && left_brother->keys.size() >= param) {
    ShiftToRightKey(node, pos);
    return;
  }
  if (right_brother && right_brother->keys.size() >= param) {
    ShiftToLeftKey(node, pos);
    return;
  }
  if (left_brother) {
    Merge(node, pos - 1);
    return;
  }
  Merge(node, pos);
}

template <typename Key, size_t param, class Compare>
void BTreeBase<Key, param, Compare>::EraseInVertex(std::shared_ptr<Node<Key>> node, size_t pos,
                                                   const Key& key) {
  auto left_child = node->childs[pos];
  auto right_child = node->childs[pos + 1];

  if (left_child->keys.size() >= param) {
    std::swap(GetMax(left_child), node->keys[pos]);
    EraseBase(left_child, key);
    --node->subtree_size;
    return;
  }
  if (right_child->keys.size() >= param) {
    std::swap(GetMin(right_child), node->keys[pos]);
    EraseBase(right_child, key);
    --node->subtree_size;
    return;
  }

  Merge(node, pos);
  EraseBase(node->childs[pos], key);
  --node->subtree_size;
}

template <typename Key, size_t param, class Compare>
bool BTreeBase<Key, param, Compare>::Exists(std::shared_ptr<Node<Key>> node, const Key& key) const {
  if (node->is_leaf) {
    auto iter = node->keys.begin();
    while (iter != node->keys.end()) {
      if (!cmp_(key, *iter) && !cmp_(*iter, key)) {
        return true;
      }
      ++iter;
    }
    return false;
  }

  auto iter_key = node->keys.begin();
  auto iter_child = node->childs.begin();
  while (iter_key != node->keys.end()) {
    if (!cmp_(key, *iter_key) && !cmp_(*iter_key, key)) {
      return true;
    }
    if (cmp_(key, *iter_key)) {
      break;
    }
    ++iter_child;
    ++iter_key;
  }
  return Exists(*iter_child, key);
}

template <typename Key, size_t param, class Compare>
std::pair<Key, bool> BTreeBase<Key, param, Compare>::Next(
    std::shared_ptr<Node<Key>> node, const Key& next, const Key& key) const {
  if (node->is_leaf) {
    for (const auto& elem : node->keys) {
      if (cmp_(key, elem)) {
        return std::make_pair(elem, true);
      }
    }
    return std::make_pair(next, true);
  }

  size_t pos = 0;
  for (const auto& elem : node->keys) {
    if (cmp_(key, elem)) {
      auto iter_child = node->childs.begin();
      std::advance(iter_child, static_cast<int>(pos));
      return Next(*iter_child, elem, key);
    }
    ++pos;
  }

  auto iter_child = node->childs.begin();
  std::advance(iter_child, static_cast<int>(pos));
  return Next(*iter_child, next, key);
}

template <typename Key, size_t param, class Compare>
std::pair<Key, bool> BTreeBase<Key, param, Compare>::Prev(
    std::shared_ptr<Node<Key>> node, const Key& prev, const Key& key) const {
  auto iter_keys = node->keys.rbegin();
  if (node->is_leaf) {
    while (iter_keys != node->keys.rend()) {
      if (cmp_(*iter_keys, key)) {
        return std::make_pair(*iter_keys, true);
      }
      ++iter_keys;
    }
    return std::make_pair(prev, true);
  }

  int64_t pos = 0;
  while (iter_keys != node->keys.rend()) {
    if (cmp_(*iter_keys, key)) {
      auto iter_child = node->childs.rbegin();
      std::advance(iter_child, pos);
      return Prev(*iter_child, *iter_keys, key);
    }
    ++pos;
    ++iter_keys;
  }

  auto iter_child = node->childs.rbegin();
  std::advance(iter_child, pos);
  return Prev(*iter_child, prev, key);
}

template <typename Key, size_t param, class Compare>
std::pair<Key, bool> BTreeBase<Key, param, Compare>::KthStatistics(
    std::shared_ptr<Node<Key>> node, size_t number) const {
  if (!node) {
    return std::make_pair(Key(), false);
  }

  if (node->is_leaf) {
    size_t pos = 0;
    for (const auto& elem : node->keys) {
      if (pos == number) {
        return std::make_pair(elem, true);
      }
      ++pos;
    }
    return std::make_pair(Key(), false);
  }

  size_t subtree_size = 0;
  auto iter = node->childs.begin();
  size_t pos = 0;
  while (iter != node->childs.end()) {
    subtree_size += GetSizeTree(*iter);
    if (subtree_size == number) {
      auto iter_ans = node->keys.begin();
      std::advance(iter_ans, static_cast<int>(pos));
      return std::make_pair(*iter_ans, true);
    }
    if (subtree_size > number) {
      auto iter_child = node->childs.begin();
      std::advance(iter_child, static_cast<int>(pos));
      return KthStatistics(*iter_child,
                           (number + GetSizeTree(*iter)) - subtree_size);
    }
    ++subtree_size;
    ++pos;
    ++iter;
  }

  return std::make_pair(Key(), false);
}

template <typename Key, size_t param, class Compare>
void BTree<Key, param, Compare>::Insert(Key&& key) {
  if (!BaseTree::root_) {
    BaseTree::root_ = std::make_shared<Node<Key>>();
    BaseTree::root_->is_leaf = true;
    BaseTree::root_->keys.push_back(std::move(key));
    BaseTree::root_->subtree_size = 1;
    return;
  }
  if (Exists(key)) {
    return;
  }
  if (BaseTree::root_->keys.size() == 2 * param - 1) {
    BaseTree::SplitRoot();
  }
  BaseTree::InsertBase(BaseTree::root_, std::move(key));
}

template <typename Key, size_t param, class Compare>
void BTree<Key, param, Compare>::Insert(const Key& key) {
  if (!BaseTree::root_) {
    BaseTree::root_ = std::make_shared<Node<Key>>();
    BaseTree::root_->is_leaf = true;
    BaseTree::root_->keys.push_back(key);
    BaseTree::root_->subtree_size = 1;
    return;
  }
  if (Exists(key)) {
    return;
  }
  if (BaseTree::root_->keys.size() == 2 * param - 1) {
    BaseTree::SplitRoot();
  }
  BaseTree::InsertBase(BaseTree::root_, key);
}

template <typename Key, size_t param, class Compare>
void BTree<Key, param, Compare>::Erase(const Key& key) {
  if (!Exists(key)) {
    return;
  }

  if (BaseTree::GetSizeTree(BaseTree::root_) == 1) {
    BaseTree::root_.reset();
    return;
  }

  if (!BaseTree::root_->is_leaf) {
    auto left_child = BaseTree::root_->childs.front();
    auto right_child = BaseTree::root_->childs.back();
    if (BaseTree::root_->keys.size() == 1 &&
        (left_child->keys.size() + 1 == param &&
         right_child->keys.size() + 1 == param)) {
      BaseTree::root_ = BaseTree::Merge(BaseTree::root_, 0);
    }
  }

  BaseTree::EraseBase(BaseTree::root_, key);
}

template <typename Key, size_t param, class Compare>
bool BTree<Key, param, Compare>::Exists(const Key& key) const {
  if (!BaseTree::root_) {
    return false;
  }
  return BaseTree::Exists(BaseTree::root_, key);
}

template <typename Key, size_t param, class Compare>
std::pair<Key, bool> BTree<Key, param, Compare>::Next(const Key& key) const {
  if (!BaseTree::root_) {
    return std::make_pair(Key(), false);
  }
  auto maximum = BaseTree::GetMax(BaseTree::root_);
  if (BaseTree::cmp_(maximum, key)) {
    return std::make_pair(Key(), false);
  }
  return BaseTree::Next(BaseTree::root_, maximum, key);
}

template <typename Key, size_t param, class Compare>
std::pair<Key, bool> BTree<Key, param, Compare>::Prev(const Key& key) const {
  if (!BaseTree::root_) {
    return std::make_pair(Key(), false);
  }
  auto minimum = BaseTree::GetMin(BaseTree::root_);
  if (BaseTree::cmp_(key, minimum)) {
    return std::make_pair(Key(), false);
  }
  return BaseTree::Prev(BaseTree::root_, minimum, key);
}

template <typename Key, size_t param, class Compare>
std::pair<Key, bool> BTree<Key, param, Compare>::KthStatistics(
    size_t number) const {
  if (!BaseTree::root_ || BaseTree::GetSizeTree(BaseTree::root_) <= number) {
    return std::make_pair(Key(), false);
  }

  return BaseTree::KthStatistics(BaseTree::root_, number);
}

template <typename Key, size_t param, class Compare>
size_t BTree<Key, param, Compare>::Size() const {
  if (!BaseTree::root_) {
    return 0;
  }
  return BaseTree::root_->subtree_size;
}

struct Less {
  bool operator()(int64_t num1, int64_t num2) const { return num1 < num2; }
};

template <size_t param>
std::string Request(BTree<int64_t, param, Less>& tree, const std::string& inputs) {
    std::string input;
    int64_t value;
    std::istringstream iss(inputs);
    iss >> input;
    iss >> value;

    if (input == "insert") {
      tree.Insert(value);
      return "";
    }

    if (input == "delete") {
      tree.Erase(value);
      return "";
    }

    if (input == "exists") {
      return tree.Exists(value) ? "true\n" : "false\n";
    }

    if (input == "next") {
      auto tmp = tree.Next(value);
      if (tmp.second) {
        std::ostringstream oss;
        oss << tmp.first << '\n';
        return oss.str();
      }
      return "none\n";
    }

    if (input == "prev") {
      auto tmp = tree.Prev(value);
      if (tmp.second) {
        std::ostringstream oss;
        oss << tmp.first << '\n';
        return oss.str();
      }
      return "none\n";
    }

    if (input == "kth") {
      auto tmp = tree.KthStatistics(value);
      if (tmp.second) {
        std::ostringstream oss;
        oss << tmp.first << '\n';
        return oss.str();
      }
      return "none\n";
    }

    return "";
}

struct InputData {
    std::vector<std::string> queries;
};

void ReadInput(InputData& input_data) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    std::string line;
    while (std::getline(std::cin, line)) {
        input_data.queries.push_back(line);
    }
}

std::vector<std::string> Solve(const InputData& input_data) {
    const size_t kConst = 10;
    BTree<int64_t, kConst, Less> tree;
    std::vector<std::string> results;
    for (size_t index = 0; index < input_data.queries.size(); ++index) {
        std::string res = Request(tree, input_data.queries[index]);
        if (!res.empty()) {
            results.push_back(res);
        }
    }
    return results;
}

void PrintOutput(const std::vector<std::string>& results) {
    for (size_t index = 0; index < results.size(); ++index) {
        std::cout << results[index];
    }
}

int main() {
    InputData input_data;
    ReadInput(input_data);
    auto res = Solve(input_data);
    PrintOutput(res);
    return 0;
}
