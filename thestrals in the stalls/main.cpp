#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

struct InputData {
  std::size_t num_elements;
  std::size_t target_count;
  std::vector<int> distances;
};

bool CheckPredicate(int result, std::size_t target_count, const std::vector<int>& distances) {
  std::size_t num_chosen = 1;
  int last_faster = distances[0];
  for (std::size_t i = 0; i < distances.size(); ++i) {
    if (distances[i] - last_faster >= result) {
      num_chosen++;
      last_faster = distances[i];
    }
    if (num_chosen >= target_count) {
      return true;
    }
  }
  return num_chosen >= target_count;
}

void ReadInput(InputData &data) {
  std::cin >> data.num_elements >> data.target_count;
  data.distances.resize(data.num_elements);
  for (std::size_t i = 0; i < data.num_elements; ++i) {
    std::cin >> data.distances[i];
  }
}

int Solve(const InputData &data) {
  int low = 0;
  int high = data.distances.back();

  while (high - low > 1) {
    int mid = (low + high) / 2;
    if (CheckPredicate(mid, data.target_count, data.distances)) {
      low = mid;
    } else {
      high = mid;
    }
  }

  return low;
}

void PrintOutput(int result) {
  std::cout << result << std::endl;
}

int main() {
  InputData data;
  ReadInput(data);
  int answer = Solve(data);
  PrintOutput(answer);
  return 0;
}
