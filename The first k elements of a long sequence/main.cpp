#include <iostream>
#include <vector>
#include <algorithm>

const int64_t kMaxValueLimit = 1073741824;

void GenerateAndOutputTopElements(std::vector<int64_t>& minHeap, size_t totalElements, 
                                   int64_t coefficient1, int64_t coefficient2, size_t totalCount) {
    int64_t newElement = 0;
    int64_t previousElement = minHeap.back();

    std::make_heap(minHeap.begin(), minHeap.end());

    for (size_t i = totalElements; i < totalCount; ++i) {
        newElement = (coefficient1 * previousElement + coefficient2) % kMaxValueLimit;

        minHeap.push_back(newElement);
        std::push_heap(minHeap.begin(), minHeap.end());

        std::pop_heap(minHeap.begin(), minHeap.end());
        minHeap.pop_back();

        previousElement = newElement;
    }

    auto comparator = [](int64_t a, int64_t b) { return a > b; };
    std::make_heap(minHeap.begin(), minHeap.end(), comparator);

    for (size_t i = 0; i < totalElements; ++i) {
        std::cout << minHeap.front() << ' ';
        std::pop_heap(minHeap.begin(), minHeap.end(), comparator);
        minHeap.pop_back();
    }
}

int main() {
    size_t totalCount;
    size_t elementsToSort;
    int64_t initialElement;
    int64_t seedMultiplier1;
    int64_t seedMultiplier2;

    std::cin >> totalCount >> elementsToSort;
    std::cin >> initialElement >> seedMultiplier1 >> seedMultiplier2;

    std::vector<int64_t> minHeap(elementsToSort, 0);
    minHeap[0] = (seedMultiplier1 * initialElement + seedMultiplier2) % kMaxValueLimit;

    for (size_t i = 1; i < elementsToSort; ++i) {
        minHeap[i] = (seedMultiplier1 * minHeap[i - 1] + seedMultiplier2) % kMaxValueLimit;
    }

    GenerateAndOutputTopElements(minHeap, elementsToSort, seedMultiplier1, seedMultiplier2, totalCount);

    return 0;
}
