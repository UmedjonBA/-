#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

struct InputData {
  size_t number_of_elements;
  size_t number_of_queries;
  std::vector<int64_t> values;
  std::vector<std::pair<size_t, size_t>> queries;
};

class SparseTable {
 public:
  SparseTable() = default;

  explicit SparseTable(const std::vector<int64_t>& input_array) {
    Build(input_array);
  }

  void Build(const std::vector<int64_t>& input_array) {
    values_ = input_array;
    size_t array_size = input_array.size();
    size_t max_log_value = GetMaxLog(array_size);
    mins_.assign(max_log_value + 1, std::vector<size_t>(array_size));

    for (size_t index_i = 0; index_i < array_size; ++index_i) {
      mins_[0][index_i] = index_i;
    }

    for (size_t index_k = 1; index_k <= max_log_value; ++index_k) {
      size_t length = 1ULL << index_k;
      size_t half = length >> 1;
      for (size_t index_i = 0; index_i + length - 1 < array_size; ++index_i) {
        size_t left_candidate = mins_[index_k - 1][index_i];
        size_t right_candidate = mins_[index_k - 1][index_i + half];
        mins_[index_k][index_i] =
            (values_[left_candidate] <= values_[right_candidate])
                ? left_candidate
                : right_candidate;
      }
    }
  }

  size_t GetMinIndex(size_t left_index, size_t right_index) const {
    size_t length = right_index - left_index + 1;
    size_t log_value = GetMaxLog(length);
    size_t left_candidate = mins_[log_value][left_index];
    size_t right_candidate =
        mins_[log_value][right_index - (1ULL << log_value) + 1];
    return (values_[left_candidate] <= values_[right_candidate])
               ? left_candidate
               : right_candidate;
  }

  int64_t GetSecondMin(size_t left_index, size_t right_index) const {
    size_t min_index = GetMinIndex(left_index, right_index);
    if (min_index == left_index) {
      return values_[GetMinIndex(left_index + 1, right_index)];
    }
    if (min_index == right_index) {
      return values_[GetMinIndex(left_index, right_index - 1)];
    }
    int64_t left_val = values_[GetMinIndex(left_index, min_index - 1)];
    int64_t right_val = values_[GetMinIndex(min_index + 1, right_index)];
    return std::min(left_val, right_val);
  }

 private:
  std::vector<int64_t> values_;
  std::vector<std::vector<size_t>> mins_;

  static size_t GetMaxLog(size_t value) {
    size_t result = 0;
    while ((1ULL << (result + 1)) <= value) {
      ++result;
    }
    return result;
  }
};

InputData ReadInput() {
  InputData data;
  std::cin >> data.number_of_elements >> data.number_of_queries;
  data.values.resize(data.number_of_elements);
  for (size_t index_i = 0; index_i < data.number_of_elements; ++index_i) {
    std::cin >> data.values[index_i];
  }
  data.queries.resize(data.number_of_queries);
  for (size_t index_i = 0; index_i < data.number_of_queries; ++index_i) {
    size_t left_index;
    size_t right_index;
    std::cin >> left_index >> right_index;
    data.queries[index_i] = {left_index - 1, right_index - 1};
  }
  return data;
}

std::unique_ptr<SparseTable> BuildSparseTable(const InputData& data) {
  auto sparse_table = std::make_unique<SparseTable>();
  sparse_table->Build(data.values);
  return std::move(sparse_table);
}

void ProcessQueries(const InputData& data, const SparseTable& sparse_table) {
  for (const auto& query : data.queries) {
    size_t left_index = query.first;
    size_t right_index = query.second;
    std::cout << sparse_table.GetSecondMin(left_index, right_index) << "\n";
  }
}

int main() {
  InputData data = ReadInput();
  auto sparse_table = BuildSparseTable(data);
  ProcessQueries(data, *sparse_table);
  return 0;
}
