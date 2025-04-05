#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
 
template <typename T>
struct Adder {
  T operator()(const T& obj1, const T& obj2) const { return obj1 + obj2; }
};
 
template <typename T, class Functor = Adder<T>>
class SegmentTreeBase {
 protected:
  SegmentTreeBase(const std::vector<T>& init_array)
      : init_size_(init_array.size()), array_(4 * init_array.size(), 0) {
    Build(init_array, 0, 0, init_size_ - 1);
  }
 
  void Build(const std::vector<int64_t>& init_array, size_t vertex, size_t left,
             size_t right) {
    if (left == right) {
      if (left >= init_size_) {
        array_[vertex] = 0;
        return;
      }
      array_[vertex] = init_array[left];
      return;
    }
 
    size_t mid = (left + right) >> 1;
    size_t first_son = (vertex << 1) + 1;
    size_t second_son = (vertex << 1) + 2;
 
    Build(init_array, first_son, left, mid);
    Build(init_array, second_son, mid + 1, right);
 
    int64_t sign = ((mid - left + 1) % 2 == 0) ? 1 : -1;
    array_[vertex] = array_[first_son] + sign * array_[second_son];
  }
 
  // void Build(const std::vector<T>& init_array, size_t vertex, size_t left,
  //            size_t right) {
  //   if (left == right) {
  //     array_[vertex] = init_array[left];
  //     return;
  //   }
 
  //   size_t mid = (left + right) >> 1;
  //   size_t first_son = 2 * vertex + 1;
  //   size_t second_son = 2 * vertex + 2;
 
  //   Build(init_array, first_son, left, mid);
  //   Build(init_array, second_son, mid + 1, right);
 
  //   array_[vertex] = func_(array_[first_son], array_[second_son]);
  // }
 
  // void Update(size_t vertex, size_t left, size_t right, size_t pos,
  //             const T& new_value) {
  //   if (left == right) {
  //     array_[vertex] = new_value;
  //     return;
  //   }
 
  //   size_t mid = (left + right) >> 1;
  //   size_t first_son = 2 * vertex + 1;
  //   size_t second_son = 2 * vertex + 2;
 
  //   if (pos <= mid) {
  //     Update(first_son, left, mid, pos, new_value);
  //   } else {
  //     Update(second_son, mid + 1, right, pos, new_value);
  //   }
 
  //   array_[vertex] = func_(array_[first_son], array_[second_son]);
  // }
 
  void Update(size_t vertex, size_t left, size_t right, size_t pos,
              int64_t new_value) {
    if (left == right) {
      array_[vertex] = new_value;
      return;
    }
 
    size_t mid = (left + right) >> 1;
    size_t first_son = (vertex << 1) + 1;
    size_t second_son = (vertex << 1) + 2;
 
    if (pos <= mid) {
      Update(first_son, left, mid, pos, new_value);
    } else {
      Update(second_son, mid + 1, right, pos, new_value);
    }
 
    int64_t sign = ((mid - left + 1) % 2 == 0) ? 1 : -1;
    array_[vertex] = array_[first_son] + sign * array_[second_son];
  }
 
  int64_t Sum(size_t vertex, size_t left_in, size_t right_in, size_t left_param,
              size_t right_param) {
    if (left_param == left_in && right_in == right_param) {
      return array_[vertex];
    }
 
    size_t mid = (left_in + right_in) >> 1;
    size_t first_son = (vertex << 1) + 1;
    size_t second_son = (vertex << 1) + 2;
    int64_t ans = 0;
 
    if (left_param <= mid) {
      ans +=
          Sum(first_son, left_in, mid, left_param, std::min(right_param, mid));
    }
    if (mid + 1 <= right_param) {
      int64_t sign = ((mid + 1 - left_param) % 2 == 0) ? 1 : -1;
      if (left_param > mid) {
        sign = 1;
      }
      ans += sign * Sum(second_son, mid + 1, right_in,
                        std::max(left_param, mid + 1), right_param);
    }
 
    return ans;
  }
 
  // int64_t Sum(size_t vertex, size_t left_in, size_t right_in, size_t
  // left_param,
  //             size_t right_param) {
  //   if (left_param == left_in && right_in == right_param) {
  //     return array_[vertex];
  //   }
 
  //   size_t mid = (left_in + right_in) >> 1;
  //   size_t first_son = 2 * vertex + 1;
  //   size_t second_son = 2 * vertex + 2;
  //   int64_t ans = 0;
 
  //   if (left_param <= mid) {
  //     ans = func_(ans, Sum(first_son, left_in, mid, left_param,
  //                          std::min(right_param, mid)));
  //   }
  //   if (mid + 1 <= right_param) {
  //     ans = func_(ans, Sum(second_son, mid + 1, right_in,
  //                          std::max(left_param, mid + 1), right_param));
  //   }
 
  //   return ans;
  // }
  Functor func_;
  size_t init_size_;
  std::vector<T> array_;
};
 
template <typename T, class Functor = Adder<T>>
class SegmentTree : protected SegmentTreeBase<T, Functor> {
 public:
  SegmentTree(const std::vector<T>& init_array)
      : SegmentTreeBase<T, Functor>(init_array) {}
 
  void Change(size_t pos, const T& value) {
    SegmentTreeBase<T, Functor>::Update(
        0, 0, SegmentTreeBase<T, Functor>::init_size_ - 1, pos, value);
  }
 
  int64_t GetSum(size_t left, size_t right) {
    return SegmentTreeBase<T, Functor>::Sum(
        0, 0, SegmentTreeBase<T, Functor>::init_size_ - 1, left, right);
  }
};
 
enum InputRequest { Change = 0, GetSum = 1 };
 
void Request(SegmentTree<int64_t>& tree) {
  int input;
  std::cin >> input;
 
  if (input == InputRequest::Change) {
    size_t pos;
    int64_t value;
    std::cin >> pos >> value;
 
    tree.Change(pos - 1, value);
  } else if (input == InputRequest::GetSum) {
    size_t left;
    size_t right;
    std::cin >> left >> right;
 
    std::cout << tree.GetSum(left - 1, right - 1) << '\n';
  }
}
 
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
 
  size_t quantity_reactions;
  std::cin >> quantity_reactions;
 
  std::vector<int64_t> reactions(quantity_reactions);
  for (auto& reaction : reactions) {
    std::cin >> reaction;
  }
 
  SegmentTree<int64_t> tree(reactions);
 
  size_t number_of_requests;
  std::cin >> number_of_requests;
  size_t request_number = 0;
 
  while (request_number < number_of_requests) {
    Request(tree);
    ++request_number;
  }
}