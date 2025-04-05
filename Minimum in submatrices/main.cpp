#include <iostream>
#include <vector>
#include <deque>

int main() {
  int n, k;
  std::cin >> n >> k;

  std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> matrix[i][j];
    }
  }

  int result_size = n - k + 1;
  std::vector<std::vector<int>> result_matrix(result_size, std::vector<int>(result_size));

  std::vector<std::vector<int>> row_mins(n, std::vector<int>(result_size));
  for (int i = 0; i < n; ++i) {
    std::deque<int> dq;
    for (int j = 0; j < n; ++j) {
      while (!dq.empty() && dq.front() < j - k + 1) {
        dq.pop_front();
      }
      while (!dq.empty() && matrix[i][dq.back()] >= matrix[i][j]) {
        dq.pop_back();
      }
      dq.push_back(j);
      if (j >= k - 1) {
        row_mins[i][j - k + 1] = matrix[i][dq.front()];
      }
    }
  }

  for (int j = 0; j < result_size; ++j) {
    std::deque<int> dq;
    for (int i = 0; i < n; ++i) {
      while (!dq.empty() && dq.front() < i - k + 1) {
        dq.pop_front();
      }
      while (!dq.empty() && row_mins[dq.back()][j] >= row_mins[i][j]) {
        dq.pop_back();
      }
      dq.push_back(i);
      if (i >= k - 1) {
        result_matrix[i - k + 1][j] = row_mins[dq.front()][j];
      }
    }
  }

  for (int i = 0; i < result_size; ++i) {
    for (int j = 0; j < result_size; ++j) {
      std::cout << result_matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}