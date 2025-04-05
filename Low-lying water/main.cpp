#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;
    std::vector<long long> heights(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> heights[i];
    }

    long long total_water = 0;
    int left = 0;
    int right = n - 1;
    long long left_max = 0;
    long long right_max = 0;

    while (left < right) {
        if (heights[left] < heights[right]) {
            if (heights[left] >= left_max) {
                left_max = heights[left];
            } else {
                total_water += left_max - heights[left];
            }
            left++;
        } else {
            if (heights[right] >= right_max) {
                right_max = heights[right];
            } else {
                total_water += right_max - heights[right];
            }
            right--;
        }
    }

    std::cout << total_water << std::endl;

    return 0;
}