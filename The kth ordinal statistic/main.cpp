#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const int64_t kMaxLimit = 10000000 + 4321;

int64_t FindKthStatistic(size_t n, size_t k, int64_t seedA0, int64_t seedA1) {
    // Используем максимальную кучу для хранения k элементов
    std::priority_queue<int64_t> maxHeap;

    maxHeap.push(seedA0);
    
    if (k == 1) {
        return seedA0;  // Если k == 1, сразу возвращаем первый элемент
    }

    maxHeap.push(seedA1);

    for (size_t i = 2; i < n; ++i) {
        int64_t nextElement = (seedA1 * 123 + seedA0 *45) % kMaxLimit;
        seedA0 = seedA1;
        seedA1 = nextElement;
        // Если размер кучи меньше k, просто добавляем элемент
        // Если больше, сравниваем с максимальным элементом в кучи
        if (maxHeap.size() < k) {
            maxHeap.push(nextElement);
        } else if (nextElement < maxHeap.top()) {
            maxHeap.pop();
            maxHeap.push(nextElement);
        }

    }

    // К концу цикла в куче будет храниться k наименьших элементов
    // верхний элемент максимальной кучи будет равен k-ой порядковой статистике
    return maxHeap.top(); // k-ая порядковая статистика
}

int main() {
    size_t n, k;
    int64_t A0, A1;

    std::cin >> n >> k;
    std::cin >> A0 >> A1;

    int64_t result = FindKthStatistic(n, k, A0, A1);
    std::cout << result << std::endl;

    return 0;
}
