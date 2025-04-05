#include <algorithm>
#include <iostream>
#include <list>
#include <ranges>
#include <vector>
 
template <class Value, class Equal, class Hash>
class UnorderedSet {
 public:
  UnorderedSet() = default;
  UnorderedSet(double rehash_coff);
 
  void Insert(const Value& value);
  void Erase(const Value& value);
  bool Find(const Value& value) const;
  std::pair<Value, bool> Get(const Value& value) const;
 
 private:
  void ReHash();
  bool FindBase(const Value& value, size_t key) const;
 
  const size_t kBaseCapacity = 4;
  const double kRehashCoff = 1.25;
  size_t size_ = 0;
  Equal equal_ = Equal();
  Hash hash_function_ = Hash();
  std::vector<std::list<Value>> table_ =
      std::vector<std::list<Value>>(kBaseCapacity);
};
 
template <class Value, class Equal, class Hash>
UnorderedSet<Value, Equal, Hash>::UnorderedSet(double rehash_coff)
    : kRehashCoff(rehash_coff) {}
 
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
std::pair<Value, bool> UnorderedSet<Value, Equal, Hash>::Get(
    const Value& value) const {
  size_t key = hash_function_(value) % table_.size();
 
  auto iter = table_[key].begin();
  while (iter != table_[key].end()) {
    if (equal_(*iter, value)) {
      return std::make_pair(*iter, true);
    }
    ++iter;
  }
 
  return std::make_pair(value, false);
}
 
template <class Value, class Equal, class Hash>
void UnorderedSet<Value, Equal, Hash>::Insert(const Value& value) {
  if (kRehashCoff * static_cast<double>(size_) >
      static_cast<double>(table_.size())) {
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
 
struct HashForPair {
  size_t operator()(const std::pair<char, size_t>& pair) const {
    return pair.first;
  }
};
 
struct EqualForPair {
  bool operator()(const std::pair<char, size_t>& pair1,
                  const std::pair<char, size_t>& pair2) const {
    return pair1.first == pair2.first;
  }
};
 
void StringConverter(std::string& string) {
  UnorderedSet<std::pair<char, size_t>, EqualForPair, HashForPair> table;
  if (string.empty()) {
    return;
  }
 
  size_t uniq_letter = 0;
  for (size_t i = 0; i < string.size(); ++i) {
    auto info = table.Get(std::make_pair(string[i], 0));
    if (info.second) {
      string[i] = 'a' + info.first.second;
    } else {
      table.Insert(std::make_pair(string[i], uniq_letter));
      string[i] = 'a' + uniq_letter;
      ++uniq_letter;
    }
  }
}
 
struct StrEqual {
  bool operator()(const std::string& str1, const std::string& str2) const {
    if (str1.size() != str2.size()) {
      return false;
    }
 
    for (size_t i = 0; i < str1.size(); ++i) {
      if (str1[i] != str2[i]) {
        return false;
      }
    }
 
    return true;
  }
};
 
void Request(UnorderedSet<std::string, StrEqual, std::hash<std::string>>&
                 unordered_set) {
  char request_type;
  std::string name;
  std::cin >> request_type >> name;
  StringConverter(name);
 
  if (request_type == '+') {
    unordered_set.Insert(name);
 
  } else if (request_type == '?') {
    if (unordered_set.Find(name)) {
      std::cout << "YES\n";
    } else {
      std::cout << "NO\n";
    }
  }
}
 
int main() { 
  size_t request_count;
  std::cin >> request_count;
 
  UnorderedSet<std::string, StrEqual, std::hash<std::string>> unordered_set;
 
  for (size_t i = 0; i < request_count; ++i) {
    Request(unordered_set);
  }
}