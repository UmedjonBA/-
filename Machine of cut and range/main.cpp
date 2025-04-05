#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
 
size_t Ceil(size_t num1, size_t num2) {
  if (num1 % num2 == 0) {
    return num1 / num2;
  }
 
  return num1 / num2 + 1;
}
 
// size_t BinSearch(const std::vector<int64_t>& array, int64_t value) {
//   int64_t left = 0;
//   int64_t right = array.size();
 
//   if (value >= array.back()) {
//     return array.size();
//   }
 
//   if (value < array.front()) {
//     return 0;
//   }
 
//   while (right - left > 1) {
//     int64_t mid = (left + right) / 2;
 
//     if (array[mid] <= value) {
//       left = mid;
//     } else {
//       right = mid;
//     }
//   }
 
//   return right;
// }
 
size_t BinSearch(const std::vector<int64_t>& array, int64_t value) {
  auto iter = std::upper_bound(array.begin(), array.end(), value);
  return iter - array.begin();
}
 
class Magic {
 public:
  Magic(const std::vector<int64_t>& init_arr, size_t bucket_size)
      : bucket_size_(bucket_size),
        buckets_(Ceil(init_arr.size(), bucket_size),
                 std::vector<int64_t>(bucket_size,
                                      std::numeric_limits<int64_t>::max())),
        buckets_sort_(buckets_) {
    auto iter = init_arr.begin();
    for (size_t i = 0; (i < buckets_.size() && iter < init_arr.end()); ++i) {
      for (size_t j = 0; (j < bucket_size && iter < init_arr.end()); ++j) {
        buckets_[i][j] = *iter;
        buckets_sort_[i][j] = *iter;
        ++iter;
      }
    }
 
    for (size_t i = 0; i < buckets_.size(); ++i) {
      std::sort(buckets_sort_[i].begin(), buckets_sort_[i].end());
    }
  }
 
  void Set(size_t pos, int64_t new_value) {
    size_t bucket_index = pos / bucket_size_;
    buckets_[bucket_index][pos % bucket_size_] = new_value;
    buckets_sort_[bucket_index] = buckets_[bucket_index];
    std::sort(buckets_sort_[bucket_index].begin(),
              buckets_sort_[bucket_index].end());
  }
 
  size_t GETLess(size_t left, size_t right, int64_t value) const {
    size_t iter = 0;
    size_t ans = 0;
 
    for (size_t i = Ceil(left, bucket_size_); i < (right + 1) / bucket_size_;
         ++i) {
      ans += BinSearch(buckets_sort_[i], value);
      iter += bucket_size_;
    }
 
    if (left % bucket_size_ != 0) {
      for (size_t i = left % bucket_size_; i < bucket_size_; ++i) {
        if (iter >= right - left + 1) {
          break;
        }
        ans += static_cast<int64_t>(buckets_[left / bucket_size_][i] <= value);
        ++iter;
      }
    }
 
    if ((left / bucket_size_ == (right + 1) / bucket_size_) &&
        (left % bucket_size_ != 0 && (right + 1) % bucket_size_ != 0)) {
      return ans;
    }
 
    if ((right + 1) % bucket_size_ != 0) {
      for (size_t i = 0; i <= right % bucket_size_; ++i) {
        ans += static_cast<int64_t>(buckets_[(right + 1) / bucket_size_][i] <=
                                    value);
      }
    }
 
    return ans;
  }
 
 private:
  size_t bucket_size_;
  std::vector<std::vector<int64_t>> buckets_;
  std::vector<std::vector<int64_t>> buckets_sort_;
};
 
void Request(Magic& magic_struct) {
  std::string input;
  std::cin >> input;
 
  if (input == "SET") {
    size_t pos;
    int64_t new_value;
    std::cin >> pos >> new_value;
    --pos;
 
    magic_struct.Set(pos, new_value);
  } else if (input == "GET") {
    size_t left;
    size_t right;
    int64_t less;
    int64_t more;
    std::cin >> left >> right >> less >> more;
    --left;
    --right;
    std::cout << magic_struct.GETLess(left, right, more) -
                     magic_struct.GETLess(left, right, less - 1)
              << '\n';
  }
}
 
int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
 
  size_t car_count;
  size_t request_count;
  std::cin >> car_count >> request_count;
 
  std::vector<int64_t> cars(car_count, 0);
  for (size_t i = 0; i < car_count; ++i) {
    std::cin >> cars[i];
  }
 
  const size_t kMagicNumber =
      std::max(static_cast<size_t>(ceil(sqrt(car_count * log2(car_count)))),
               static_cast<size_t>(1));
  Magic magic_struct(cars, kMagicNumber);
 
  size_t request = 0;
  while (request < request_count) {
    Request(magic_struct);
    ++request;
  }
}