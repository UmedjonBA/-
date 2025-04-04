#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <stdexcept>

struct InputData {
    size_t total_requests;
    struct Request {
        std::string command;
        size_t value = 0;
        size_t vertex_index = 0;
        int64_t delta = 0;
    };
    std::vector<Request> requests;
};

class MinHeap {
public:
    std::shared_ptr<std::pair<int64_t, size_t>> Add(int64_t value) {
        auto node = std::make_shared<std::pair<int64_t, size_t>>(value, elements_.size());
        elements_.push_back(node);
        UpHeap(elements_.size() - 1);
        return node;
    }

    int64_t GetMinimum() const { 
        if (elements_.empty()) {
            throw std::runtime_error("Heap is empty.");
        }
        return elements_.front()->first; 
    }

    void RemoveMinimum() {
        if (elements_.empty()) {
            throw std::runtime_error("Heap is empty.");
        }
        std::swap(elements_[0], elements_.back());
        std::swap(elements_[0]->second, elements_.back()->second);
        elements_.pop_back();
        if (!elements_.empty()) {
            DownHeap(0);
        }
    }

    void UpdateKey(size_t heap_index, int64_t delta_value) {
        if (heap_index >= elements_.size()) {
            throw std::out_of_range("Index out of range in UpdateKey.");
        }
        elements_[heap_index]->first -= delta_value;
        UpHeap(heap_index);
    }

private:
    std::vector<std::shared_ptr<std::pair<int64_t, size_t>>> elements_;

    void UpHeap(size_t current_index) {
        if (current_index == 0) {
            return;
        }
        size_t parent_index = (current_index - 1) / 2;
        if (elements_[current_index]->first < elements_[parent_index]->first) {
            std::swap(elements_[current_index], elements_[parent_index]);
            std::swap(elements_[current_index]->second, elements_[parent_index]->second);
            UpHeap(parent_index);
        }
    }

    void DownHeap(size_t current_index) {
        size_t left_child_index = 2 * current_index + 1;
        size_t right_child_index = 2 * current_index + 2;
        size_t smallest_index = current_index;

        if (left_child_index < elements_.size() && elements_[left_child_index]->first < elements_[smallest_index]->first) {
            smallest_index = left_child_index;
        }

        if (right_child_index < elements_.size() && elements_[right_child_index]->first < elements_[smallest_index]->first) {
            smallest_index = right_child_index;
        }

        if (smallest_index != current_index) {
            std::swap(elements_[current_index], elements_[smallest_index]);
            std::swap(elements_[current_index]->second, elements_[smallest_index]->second);
            DownHeap(smallest_index);
        }
    }
};

InputData ReadInput() {
    InputData input;
    std::cin >> input.total_requests;
    input.requests.resize(input.total_requests);
    for (size_t request_idx = 0; request_idx < input.total_requests; ++request_idx) {
        std::cin >> input.requests[request_idx].command;
        if (input.requests[request_idx].command == "insert") {
            std::cin >> input.requests[request_idx].value;
        } else if (input.requests[request_idx].command == "decreaseKey") {
            std::cin >> input.requests[request_idx].vertex_index >> input.requests[request_idx].delta;
        }
    }
    return input;
}

void ProcessRequest(MinHeap& heap, const InputData::Request& req,
                   std::vector<std::shared_ptr<std::pair<int64_t, size_t>>>& vertex_tracker,
                   size_t request_number) {
    if (req.command == "insert") {
        vertex_tracker[request_number] = heap.Add(req.value);
    } else if (req.command == "getMin") {
        try {
            std::cout << heap.GetMinimum() << '\n';
        } catch (...) {
            std::cout << "error\n";
        }
    } else if (req.command == "extractMin") {
        try {
            heap.RemoveMinimum();
        } catch (...) {
            std::cout << "error\n";
        }
    } else if (req.command == "decreaseKey") {
        if (req.vertex_index == 0 || req.vertex_index > vertex_tracker.size() || !vertex_tracker[req.vertex_index - 1]) {
            std::cerr << "error\n";
            return;
        }
        size_t heap_index = vertex_tracker[req.vertex_index - 1]->second;
        try {
            heap.UpdateKey(heap_index, req.delta);
        } catch (...) {
            std::cerr << "error\n";
        }
    }
}

std::vector<int64_t> Solve(const InputData& input) {
    MinHeap heap;
    std::vector<std::shared_ptr<std::pair<int64_t, size_t>>> vertex_tracker(input.total_requests, nullptr);
    for (size_t request_idx = 0; request_idx < input.total_requests; ++request_idx) {
        ProcessRequest(heap, input.requests[request_idx], vertex_tracker, request_idx);
    }
    return {};
}

void PrintOutput() {}

int main() {
    InputData input = ReadInput();
    std::vector<int64_t> result = Solve(input);
    PrintOutput();
    return 0;
}
