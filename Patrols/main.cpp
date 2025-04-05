#include <iostream>
#include <vector>

using namespace std;

class PatrolStreet {
private:
    vector<int> streetSegments;  // Сегменты улицы
    long long totalTax;          // Общая сумма квадратов длин
public:
    // Конструктор класса
    PatrolStreet(const vector<int>& segments) {
        streetSegments = segments;
        totalTax = 0;
        // Вычисление начальной суммы квадратов длин
        for (int length : streetSegments) {
            totalTax += static_cast<long long>(length) * length;
        }
    }

    // Вспомогательная функция для вычитания квадрата длины из общей суммы
    void subtractFromTotalTax(int length) {
        totalTax -= static_cast<long long>(length) * length;
    }

    // Вспомогательная функция для добавления квадрата длины в общую сумму
    void addToTotalTax(int length) {
        totalTax += static_cast<long long>(length) * length;
    }

    // Обработка удаления группировки с двумя соседями
    void handleRemovalWithTwoNeighbors(int idx, int groupLength) {
        int leftLength = streetSegments[idx - 1];
        int rightLength = streetSegments[idx + 1];

        subtractFromTotalTax(leftLength);
        subtractFromTotalTax(rightLength);

        int leftPart = groupLength / 2;
        int rightPart = groupLength - leftPart;

        streetSegments[idx - 1] += leftPart;
        streetSegments[idx + 1] += rightPart;

        addToTotalTax(streetSegments[idx - 1]);
        addToTotalTax(streetSegments[idx + 1]);
    }

    // Обработка удаления группировки с левым соседом
    void handleRemovalWithLeftNeighbor(int idx, int groupLength) {
        subtractFromTotalTax(streetSegments[idx - 1]);
        streetSegments[idx - 1] += groupLength;
        addToTotalTax(streetSegments[idx - 1]);
    }

    // Обработка удаления группировки с правым соседом
    void handleRemovalWithRightNeighbor(int idx, int groupLength) {
        subtractFromTotalTax(streetSegments[idx + 1]);
        streetSegments[idx + 1] += groupLength;
        addToTotalTax(streetSegments[idx + 1]);
    }

    // Функция для удаления группировки
    void removePatrolGroup(int idx) {
        int groupLength = streetSegments[idx];
        subtractFromTotalTax(groupLength);

        bool leftNeighborExists = (idx > 0);
        bool rightNeighborExists = (idx < static_cast<int>(streetSegments.size()) - 1);

        if (leftNeighborExists && rightNeighborExists) {
            handleRemovalWithTwoNeighbors(idx, groupLength);
        } else if (leftNeighborExists) {
            handleRemovalWithLeftNeighbor(idx, groupLength);
        } else if (rightNeighborExists) {
            handleRemovalWithRightNeighbor(idx, groupLength);
        }

        streetSegments.erase(streetSegments.begin() + idx);
    }

    // Функция для разделения сегмента
    void splitSegment(int idx) {
        int originalLength = streetSegments[idx];
        int leftSplit = originalLength / 2;
        int rightSplit = originalLength - leftSplit;

        subtractFromTotalTax(originalLength);
        addToTotalTax(leftSplit);
        addToTotalTax(rightSplit);

        streetSegments[idx] = leftSplit;
        streetSegments.insert(streetSegments.begin() + idx + 1, rightSplit);
    }

    // Получение общей суммы квадратов длин
    long long getTotalTax() const {
        return totalTax;
    }

    // Получение текущих сегментов улицы
    const vector<int>& getSegments() const {
        return streetSegments;
    }
};

int main() {
    int n, p;  // Чтение входных данных (n - количество отрезков, p - номер подзадачи)
    cin >> n >> p;

    vector<int> segments(n);
    for (int i = 0; i < n; ++i) {
        cin >> segments[i];
    }

    // Создаем объект класса PatrolStreet
    PatrolStreet patrolStreet(segments);

    int q;  // Чтение количества событий
    cin >> q;

    vector<long long> results;
    results.push_back(patrolStreet.getTotalTax());

    // Обработка событий
    while (q--) {
        int eventType, segmentIdx;
        cin >> eventType >> segmentIdx;
        --segmentIdx;  // Приведение индекса к нулевой системе

        if (eventType == 1) {
            patrolStreet.removePatrolGroup(segmentIdx);
        } else if (eventType == 2) {
            patrolStreet.splitSegment(segmentIdx);
        }

        results.push_back(patrolStreet.getTotalTax());
    }

    // Вывод всех результатов
    for (long long res : results) {
        cout << res << endl;
    }

    return 0;
}
