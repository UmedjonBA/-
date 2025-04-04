#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

namespace {
  const int kPrecision = 6;  // Перенесено в анонимный неймспейс
}

struct InputData {
  std::size_t array_size;
  std::vector<double> numbers;
  std::size_t query_count;
  std::vector<std::pair<std::size_t, std::size_t>> queries;
};

void ReadInput(InputData &data) {
  std::cin >> data.array_size;
  data.numbers.resize(data.array_size);

  for (std::size_t index = 0; index < data.array_size; ++index) {
    std::cin >> data.numbers[index];
  }

  std::cin >> data.query_count;
  data.queries.resize(data.query_count);
  for (std::size_t query_index = 0; query_index < data.query_count; ++query_index) {
    std::size_t left, right;
    std::cin >> left >> right;
    data.queries[query_index] = {left, right};
  }
}

std::vector<double> Solve(const InputData &data) {
  std::vector<double> prefix_log_sum(data.array_size);
  for (std::size_t index = 0; index < data.array_size; ++index) {
    if (index > 0) {
      prefix_log_sum[index] = prefix_log_sum[index - 1] + std::log(data.numbers[index]);
    } else {
      prefix_log_sum[index] = std::log(data.numbers[index]);
    }
  }

  std::vector<double> query_results(data.query_count);
  for (std::size_t query_index = 0; query_index < data.query_count; ++query_index) {
    std::size_t left = data.queries[query_index].first;
    std::size_t right = data.queries[query_index].second;

    if (left > 0) {
      query_results[query_index] =
          std::exp((prefix_log_sum[right] - prefix_log_sum[left - 1]) /
                   (static_cast<double>(right - left + 1)));
    } else {
      query_results[query_index] =
          std::exp(prefix_log_sum[right] / static_cast<double>(right + 1));
    }
  }

  return query_results;
}

void PrintResults(const std::vector<double> &results) {
  std::cout << std::fixed << std::setprecision(kPrecision);
  for (std::size_t i = 0; i < results.size(); ++i) {
    std::cout << results[i] << "\n";
  }
}

int main() {
  InputData data;
  ReadInput(data);
  std::vector<double> results = Solve(data);
  PrintResults(results);
  return 0;
}
