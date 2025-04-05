#include <algorithm>
#include <iostream>
#include <list>
#include <ranges>
#include <vector>
 
template <class Value, class Equal, class Hash>
class UnorderedMultiSet {
 public:
  UnorderedMultiSet() = default;
  UnorderedMultiSet(size_t base_size);
  UnorderedMultiSet(double rehash_coff);
 
  void Insert(const Value& value);
  void Erase(const Value& value);
  bool Find(const Value& value) const;
  size_t GetCount(const Value& value) const;
 
 private:
  void ReHash();
 
  const size_t kBaseCapacity = 16;
  const double kRehashCoff = 1;
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
UnorderedMultiSet<Value, Equal, Hash>::UnorderedMultiSet(size_t base_size)
    : kBaseCapacity(base_size) {}
 
template <class Value, class Equal, class Hash>
bool UnorderedMultiSet<Value, Equal, Hash>::Find(const Value& value) const {
  size_t key = hash_function_(value) % table_.size();
  return std::ranges::any_of(table_[key].begin(), table_[key].end(),
                             [&](const std::pair<Value, size_t>& elem) {
                               return equal_(value, elem.first);
                             });
}
 
template <class Value, class Equal, class Hash>
size_t UnorderedMultiSet<Value, Equal, Hash>::GetCount(
    const Value& value) const {
  size_t key = hash_function_(value) % table_.size();
 
  auto iter = table_[key].begin();
  while (iter != table_[key].end()) {
    if (equal_(iter->first, value)) {
      return iter->second;
    }
    ++iter;
  }
 
  return 0;
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
  std::vector<std::list<std::pair<Value, size_t>>> new_table(4 * table_.size());
 
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
 
struct Point {
  int64_t cord_x;
  int64_t cord_y;
};
 
size_t SquareDistance(const Point& point1, const Point& point2) {
  return (point1.cord_x - point2.cord_x) * (point1.cord_x - point2.cord_x) +
         (point1.cord_y - point2.cord_y) * (point1.cord_y - point2.cord_y);
}
 
struct Equal {
  bool operator()(size_t num1, size_t num2) const { return num1 == num2; }
};
 
size_t CalckIsoscelesTrianglesForPoint(const std::vector<Point>& points,
                                       const Point& point) {
  UnorderedMultiSet<size_t, Equal, std::hash<size_t>> table(points.size());
  std::vector<size_t> distances(points.size());
  size_t uniq_elem = 0;
  for (const auto& other_point : points) {
    auto distance = SquareDistance(point, other_point);
    if (!table.Find(distance)) {
      distances[uniq_elem] = distance;
      ++uniq_elem;
    }
    table.Insert(distance);
  }
 
  size_t ans = 0;
  for (size_t i = 0; i < uniq_elem; ++i) {
    auto count = table.GetCount(distances[i]);
    ans += (count * (count - 1)) / 2;
  }
 
  return ans;
}
 
int main() {
  size_t points_count;
  std::cin >> points_count;
  int request_type;
  std::cin >> request_type;
 
  std::vector<Point> points(points_count);
  for (auto& point : points) {
    std::cin >> point.cord_x >> point.cord_y;
  }
 
  if (request_type == 2) {
    std::cout << 0 << '\n';
    return 0;
  }
 
  size_t isosceles_triangles_count = 0;
  for (const auto& point : points) {
    isosceles_triangles_count += CalckIsoscelesTrianglesForPoint(points, point);
  }
 
  std::cout << isosceles_triangles_count << '\n';
}