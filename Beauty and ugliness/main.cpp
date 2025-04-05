#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n, k;
    std::cin >> n >> k;
    std::vector<int> coins(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> coins[i];
    }

    int max_len = 0;
    int left = 0;
    int zeros_count = 0;

    for (int right = 0; right < n; ++right) {
        if (coins[right] == 0) {
            zeros_count++;
        }

        while (zeros_count > k) {
            if (coins[left] == 0) {
                zeros_count--;
            }
            left++;
        }
        
        max_len = std::max(max_len, right - left + 1);
    }

    std::cout << max_len << std::endl;

    return 0;
}