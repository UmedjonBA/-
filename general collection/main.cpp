#include <algorithm>
#include <iostream>
#include <list>
#include <ranges>
#include <vector>
 
template <class Value, class Equal, class Hash>
class UnorderedMultiSet {
 public:
  UnorderedMultiSet() = default;
  UnorderedMultiSet(double rehash_coff);
 
  void Insert(const Value& value);
  void Erase(const Value& value);
  bool Find(const Value& value) const;
 
 private:
  void ReHash();
 
  const size_t kBaseCapacity = 4;
  const double kRehashCoff = 1.25;
  size_t capacity_ = 0;
  Equal equal_ = Equal();
  Hash hash_function_ = Hash();
  std::vector<std::list<std::pair<Value, size_t>>> table_ =
      std::vector<std::list<std::pair<Value, size_t>>>(kBaseCapacity);
};
 
template <class Value, class Equal, class Hash>
UnorderedMultiSet<Value, Equal, Hash>::UnorderedMultiSet(double rehash_coff)
    : kRehashCoff(rehash_coff) {}
 
template <class Value, class Equal, class Hash>
bool UnorderedMultiSet<Value, Equal, Hash>::Find(const Value& value) const {
  size_t key = hash_function_(value) % table_.size();
  return std::ranges::any_of(table_[key].begin(), table_[key].end(),
                             [&](const std::pair<Value, size_t>& elem) {
                               return equal_(value, elem.first);
                             });
}
 
template <class Value, class Equal, class Hash>
void UnorderedMultiSet<Value, Equal, Hash>::Insert(const Value& value) {
  size_t key = hash_function_(value) % table_.size();
  auto iter = table_[key].begin();
  while (iter != table_[key].end()) {
    if (equal_(iter->first, value)) {
      ++iter->second;
      return;
    }
    ++iter;
  }
 
  if (kRehashCoff * static_cast<double>(capacity_) >
      static_cast<double>(table_.size())) {
    ReHash();
  }
 
  key = hash_function_(value) % table_.size();
  table_[key].push_back(std::pair<Value, size_t>(value, 1));
  ++capacity_;
}
 
template <class Value, class Equal, class Hash>
void UnorderedMultiSet<Value, Equal, Hash>::Erase(const Value& value) {
  size_t key = hash_function_(value) % table_.size();
 
  auto iter = table_[key].begin();
  while (iter != table_[key].end()) {
    if (equal_(iter->first, value)) {
      if (iter->second == 1) {
        table_[key].erase(iter);
        --capacity_;
      } else {
        --iter->second;
      }
      break;
    }
    ++iter;
  }
}
 
template <class Value, class Equal, class Hash>
void UnorderedMultiSet<Value, Equal, Hash>::ReHash() {
  std::vector<std::list<std::pair<Value, size_t>>> new_table(2 * table_.size());
 
  size_t new_key;
  hash_function_ = Hash();
  for (const auto& list : table_) {
    for (const auto& value : list) {
      new_key = hash_function_(value.first) % new_table.size();
      new_table[new_key].push_back(value);
    }
  }
 
  table_ = new_table;
}
 
struct HashFunctor {
  size_t operator()(int64_t value) const { return std::abs(value); }
};
 
struct Equal {
  bool operator()(int64_t num1, int64_t num2) const { return num1 == num2; }
};
 
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
 
  size_t first_collection_size;
  std::cin >> first_collection_size;
  UnorderedMultiSet<int64_t, Equal, HashFunctor> first_collection;
  for (size_t i = 0; i < first_collection_size; ++i) {
    int64_t collection_elem;
    std::cin >> collection_elem;
    first_collection.Insert(collection_elem);
  }
 
  std::vector<int64_t> intersection_collection;
 
  size_t second_collection_size;
  std::cin >> second_collection_size;
  for (size_t i = 0; i < second_collection_size; ++i) {
    int64_t collection_elem;
    std::cin >> collection_elem;
    if (first_collection.Find(collection_elem)) {
      intersection_collection.push_back(collection_elem);
      first_collection.Erase(collection_elem);
    }
  }
 
  std::cout << intersection_collection.size() << '\n';
  for (size_t i = 0; i < intersection_collection.size(); ++i) {
    std::cout << intersection_collection[i] << ' ';
  }
}
