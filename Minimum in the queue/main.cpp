#include <algorithm>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

struct InputData {
    size_t total_requests;
    struct Request {
        std::string command;
        int64_t value = 0;
        bool has_value = false;
    };
    std::vector<Request> requests;
};

template <typename T>
struct CustomStack {
  std::stack<std::pair<T, T>> elements;

  void Add(T value) {
    if (elements.empty() || value < elements.top().second) {
      elements.emplace(value, value);
      return;
    }
    elements.emplace(value, elements.top().second);
  }

  void Remove() {
    elements.pop();
  }

  T GetTop() const {
    return elements.top().first;
  }

  T GetMinimum() const {
    return elements.top().second;
  }

  size_t GetSize() const {
    return elements.size();
  }
};

template <typename T>
class CustomQueue {
  CustomStack<T> frontStack_;
  CustomStack<T> backStack_;

  void Transfer() {
    while (frontStack_.GetSize() != 0) {
      backStack_.Add(frontStack_.GetTop());
      frontStack_.Remove();
    }
  }

 public:
  size_t GetSize() const {
    return backStack_.GetSize() + frontStack_.GetSize();
  }

  void Enqueue(T value) {
    frontStack_.Add(value);
  }

  T Dequeue() {
    if (GetSize() == 0) {
      return static_cast<T>(-1);
    }
    if (backStack_.GetSize() == 0) {
      Transfer();
    }
    T result = backStack_.GetTop();
    backStack_.Remove();
    return result;
  }

  T GetMinimum() const {
    if (GetSize() == 0) {
      return static_cast<T>(-1);
    }
    if (frontStack_.GetSize() == 0) {
      return backStack_.GetMinimum();
    }
    if (backStack_.GetSize() == 0) {
      return frontStack_.GetMinimum();
    }
    return std::min(frontStack_.GetMinimum(), backStack_.GetMinimum());
  }

  T Front() {
    if (GetSize() == 0) {
      return static_cast<T>(-1);
    }
    if (backStack_.GetSize() == 0) {
      Transfer();
    }
    return backStack_.GetTop();
  }

  void ClearAll() {
    while (frontStack_.GetSize() != 0) {
      frontStack_.Remove();
    }
    while (backStack_.GetSize() != 0) {
      backStack_.Remove();
    }
  }
};

InputData ReadInput() {
    InputData input;
    std::cin >> input.total_requests;
    input.requests.resize(input.total_requests);
    for (size_t index = 0; index < input.total_requests; ++index) {
        std::cin >> input.requests[index].command;
        if (input.requests[index].command == "enqueue") {
            int64_t val;
            std::cin >> val;
            input.requests[index].value = val;
            input.requests[index].has_value = true;
        }
    }
    return input;
}

std::vector<std::string> Solve(const InputData& input) {
    CustomQueue<int64_t> queue;
    std::vector<std::string> results;
    results.reserve(input.total_requests);

    for (size_t index = 0; index < input.total_requests; ++index) {
        const auto& req = input.requests[index];
        if (req.command == "enqueue") {
            queue.Enqueue(req.value);
            results.push_back("ok");
        } else if (req.command == "dequeue") {
            int64_t result = queue.Dequeue();
            if (result == -1) {
                results.push_back("error");
            } else {
                results.push_back(std::to_string(result));
            }
        } else if (req.command == "front") {
            int64_t result = queue.Front();
            if (result == -1) {
                results.push_back("error");
            } else {
                results.push_back(std::to_string(result));
            }
        } else if (req.command == "size") {
            results.push_back(std::to_string(queue.GetSize()));
        } else if (req.command == "clear") {
            queue.ClearAll();
            results.push_back("ok");
        } else if (req.command == "min") {
            int64_t result = queue.GetMinimum();
            if (result == -1) {
                results.push_back("error");
            } else {
                results.push_back(std::to_string(result));
            }
        }
    }

    return results;
}

void PrintOutput(const std::vector<std::string>& results) {
    for (const auto& res : results) {
        std::cout << res << "\n";
    }
}

int main() {
    InputData input = ReadInput();
    std::vector<std::string> result = Solve(input);
    PrintOutput(result);
    return 0;
}
