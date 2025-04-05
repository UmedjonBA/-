#include <iostream>
#include <vector>
#include <unordered_map>

int main() {
    int n;
    std::cin >> n;
    long long k;
    std::cin >> k;
    std::vector<long long> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    long long count = 0;
    long long current_sum = 0;
    std::unordered_map<long long, int> sum_counts;
    sum_counts[0] = 1;

    for (int i = 0; i < n; ++i) {
        current_sum += arr[i];
        if (sum_counts.find(current_sum - k) != sum_counts.end()) {
            count += sum_counts[current_sum - k];
        }
        sum_counts[current_sum]++;
    }
    
    std::cout << count << std::endl;

    return 0;
}