#include <iostream>
#include <vector>

template <typename T>
void Quicksort(std::vector<T>& arr, size_t left, size_t right) {
    if (left >= right) {
        return;
    }
    size_t pivot = (left + right) / 2;
    T pivot_value = arr[pivot];
    size_t leftIndex = left;
    size_t rightIndex = right;
    while (leftIndex <= rightIndex) {
        while (arr[leftIndex] < pivot_value) {
            ++leftIndex;
        }
        while (arr[rightIndex] > pivot_value) {
            --rightIndex;
        }
        if (leftIndex <= rightIndex) {
            T temp = arr[leftIndex];
            arr[leftIndex] = arr[rightIndex];
            arr[rightIndex] = temp;
            ++leftIndex;
            if (rightIndex > 0) {
                --rightIndex;
            }
        }
    }
    if (rightIndex > left) {
        Quicksort(arr, left, rightIndex);
    }
    if (leftIndex < right) {
        Quicksort(arr, leftIndex, right);
    }
}

struct InputData {
    size_t n;
    std::vector<int64_t> arr;
};

InputData ReadInput() {
    InputData input;
    std::cin >> input.n;
    input.arr.resize(input.n);
    for (size_t index = 0; index < input.n; ++index) {
        std::cin >> input.arr[index];
    }
    return input;
}

std::vector<int64_t> Solve(const InputData& input) {
    std::vector<int64_t> result = input.arr;
    if (result.size() > 1) {
        Quicksort(result, 0, result.size() - 1);
    }
    return result;
}

void PrintOutput(const std::vector<int64_t>& arr) {
    for (const auto& num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n";
}

int main() {
    InputData input = ReadInput();
    std::vector<int64_t> result = Solve(input);
    PrintOutput(result);
    return 0;
}
