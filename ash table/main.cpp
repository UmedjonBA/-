#include <algorithm>
#include <iostream>
#include <list>
#include <ranges>
#include <vector>
 
template <class Value, class Equal, class Hash>
class UnorderedSet {
 public:
  UnorderedSet() = default;
  UnorderedSet(size_t rehash_coff);
 
  void Insert(const Value& value);
  void Erase(const Value& value);
  bool Find(const Value& value) const;
 
 private:
  void ReHash();
  bool FindBase(const Value& value, size_t key) const;
 
  const size_t kBaseCapacity = 4;
  size_t rehash_coff_ = 2;
  size_t size_ = 0;
  Equal equal_ = Equal();
  Hash hash_function_ = Hash();
  std::vector<std::list<Value>> table_ =
      std::vector<std::list<Value>>(kBaseCapacity);
};
 
template <class Value, class Equal, class Hash>
UnorderedSet<Value, Equal, Hash>::UnorderedSet(size_t rehash_coff)
    : rehash_coff_(rehash_coff) {}
 
template <class Value, class Equal, class Hash>
bool UnorderedSet<Value, Equal, Hash>::FindBase(const Value& value,
                                                size_t key) const {
  return std::ranges::any_of(
      table_[key].begin(), table_[key].end(),
      [&](const Value& elem) { return equal_(value, elem); });
}
 
template <class Value, class Equal, class Hash>
bool UnorderedSet<Value, Equal, Hash>::Find(const Value& value) const {
  size_t key = hash_function_(value) % table_.size();
  return FindBase(value, key);
}
 
template <class Value, class Equal, class Hash>
void UnorderedSet<Value, Equal, Hash>::Insert(const Value& value) {
  if (rehash_coff_ * size_ > table_.size()) {
    ReHash();
  }
 
  size_t key = hash_function_(value) % table_.size();
  if (FindBase(value, key)) {
    return;
  }
 
  table_[key].push_back(value);
  ++size_;
}
 
template <class Value, class Equal, class Hash>
void UnorderedSet<Value, Equal, Hash>::Erase(const Value& value) {
  size_t key = hash_function_(value) % table_.size();
 
  auto iter = table_[key].begin();
  while (iter != table_[key].end()) {
    if (equal_(*iter, value)) {
      table_[key].erase(iter);
      --size_;
      break;
    }
    ++iter;
  }
}
 
template <class Value, class Equal, class Hash>
void UnorderedSet<Value, Equal, Hash>::ReHash() {
  std::vector<std::list<Value>> new_table(2 * table_.size());
 
  size_t new_key;
  hash_function_ = Hash();
  for (const auto& list : table_) {
    for (const auto& value : list) {
      new_key = hash_function_(value) % new_table.size();
      new_table[new_key].push_back(value);
    }
  }
 
  table_ = new_table;
}
 
struct HashFunctor {
  size_t operator()(int64_t value) const { return 4 * std::abs(value) + 4; }
};
 
struct Equal {
  bool operator()(int64_t num1, int64_t num2) const { return num1 == num2; }
};
 
void Request(UnorderedSet<int64_t, Equal, HashFunctor>& set) {
  char input;
  std::cin >> input;
  int64_t value;
  std::cin >> value;
  if (input == '+') {
    set.Insert(value);
  } else if (input == '-') {
    set.Erase(value);
 
  } else if (input == '?') {
    if (set.Find(value)) {
      std::cout << "YES\n";
    } else {
      std::cout << "NO\n";
    }
  }
}
 
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
 
  size_t request_count;
  std::cin >> request_count;
 
  UnorderedSet<int64_t, Equal, HashFunctor> set;
 
  for (size_t i = 0; i < request_count; ++i) {
    Request(set);
  }
}