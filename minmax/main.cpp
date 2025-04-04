#include <iostream>
#include <limits>
#include <vector>

using PairIndex = std::pair<size_t, size_t>;

template <bool isMinHeap>
class Heap {
 public:
  void Insert(std::pair<int64_t, PairIndex>* element) {
    data_.push_back(element);
    SiftUp(data_.size() - 1);
  }

  int64_t GetTop() const { return data_.front()->first; }
  size_t GetSize() const { return data_.size(); }

  std::pair<int64_t, PairIndex>* RemoveTop(bool isDelete) {
    std::swap(data_[0], data_.back());
    UpdateIndex(data_[0]->second, data_.back()->second);

    auto* temp = data_.back();
    data_.pop_back();
    SiftDown(0);

    if (isDelete) {
      delete temp;
      return nullptr;
    }
    return temp;
  }

  void DecreaseKey(size_t idx, int64_t delta) {
    data_[idx]->first -= delta;
    SiftUp(idx);
  }

  void UpdateKey(size_t idx, int64_t newValue) {
    data_[idx]->first = newValue;
    SiftUp(idx);
  }

  void Clear() {
    for (const auto& element : data_) {
      data_.pop_back();
      delete element;
    }
  }

 private:
  std::vector<std::pair<int64_t, PairIndex>*> data_;

  static bool Compare(int64_t a, int64_t b) {
    return isMinHeap ? (a < b) : (a > b);
  }

  static void UpdateIndex(PairIndex& a, PairIndex& b) {
    if (isMinHeap) {
      std::swap(a.first, b.first);
    } else {
      std::swap(a.second, b.second);
    }
  }

  void SiftUp(size_t idx) {
    if (idx == 0) return;
    size_t parentIdx = (idx - 1) / 2;
    if (Compare(data_[idx]->first, data_[parentIdx]->first)) {
      std::swap(data_[idx], data_[parentIdx]);
      UpdateIndex(data_[idx]->second, data_[parentIdx]->second);
      SiftUp(parentIdx);
    }
  }

  void SiftDown(size_t idx) {
    if (2 * idx + 1 >= data_.size()) return;
    size_t childIdx = 2 * idx + 1;
    if (childIdx + 1 < data_.size() && Compare(data_[childIdx + 1]->first, data_[childIdx]->first)) {
      ++childIdx;
    }
    if (Compare(data_[childIdx]->first, data_[idx]->first)) {
      std::swap(data_[idx], data_[childIdx]);
      UpdateIndex(data_[idx]->second, data_[childIdx]->second);
      SiftDown(childIdx);
    }
  }
};

enum ResultType { Error, Success };

class MinMaxQueue {
 public:
  void Insert(int64_t value) {
    auto* newNode = new std::pair<int64_t, PairIndex>(value, {GetSize(), GetSize()});
    maxHeap_.Insert(newNode);
    minHeap_.Insert(newNode);
  }

  std::pair<ResultType, int64_t> GetMin() const {
    if (GetSize() == 0) return {Error, 0};
    return {Success, minHeap_.GetTop()};
  }

  std::pair<ResultType, int64_t> GetMax() const {
    if (GetSize() == 0) return {Error, 0};
    return {Success, maxHeap_.GetTop()};
  }

  size_t GetSize() const { return maxHeap_.GetSize(); }

  void Clear() {
    while (minHeap_.GetSize() > 0) {
      minHeap_.RemoveTop(false);
    }
    maxHeap_.Clear();
  }

  ResultType ExtractMax() {
    if (GetSize() == 0) return Error;
    auto* maxElement = maxHeap_.RemoveTop(false);
    minHeap_.UpdateKey(maxElement->second.first, std::numeric_limits<int64_t>::min());
    minHeap_.RemoveTop(true);
    return Success;
  }

  ResultType ExtractMin() {
    if (GetSize() == 0) return Error;
    auto* minElement = minHeap_.RemoveTop(false);
    maxHeap_.UpdateKey(minElement->second.second, std::numeric_limits<int64_t>::max());
    maxHeap_.RemoveTop(true);
    return Success;
  }

  ~MinMaxQueue() { Clear(); }

 private:
  Heap<true> minHeap_;
  Heap<false> maxHeap_;
};

void ProcessRequest(MinMaxQueue& queue, const std::string& command) {
  if (command == "insert") {
    int64_t value;
    std::cin >> value;
    queue.Insert(value);
    std::cout << "ok\n";

  } else if (command == "get_min") {
    auto result = queue.GetMin();
    if (result.first == Error) {
      std::cout << "error\n";
    } else {
      std::cout << result.second << '\n';
    }

  } else if (command == "extract_min") {
    auto minValue = queue.GetMin();
    auto result = queue.ExtractMin();
    if (result == Error) {
      std::cout << "error\n";
    } else {
      std::cout << minValue.second << '\n';
    }

  } else if (command == "extract_max") {
    auto maxValue = queue.GetMax();
    auto result = queue.ExtractMax();
    if (result == Error) {
      std::cout << "error\n";
    } else {
      std::cout << maxValue.second << '\n';
    }

  } else if (command == "get_max") {
    auto result = queue.GetMax();
    if (result.first == Error) {
      std::cout << "error\n";
    } else {
      std::cout << result.second << '\n';
    }

  } else if (command == "size") {
    std::cout << queue.GetSize() << '\n';

  } else if (command == "clear") {
    queue.Clear();
    std::cout << "ok\n";
  }
}

int main() {
  size_t numberOfCommands;
  std::cin >> numberOfCommands;
  MinMaxQueue queue;

  for (size_t i = 0; i < numberOfCommands; ++i) {
    std::string command;
    std::cin >> command;
    ProcessRequest(queue, command);
  }
}
