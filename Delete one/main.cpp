#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;
    std::vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    int max_len = 0;
    int left_ones = 0;
    int right_ones = 0;
    int zero_index = -1;
    
    for (int i = 0; i < n; ++i){
        if (arr[i] == 1){
            right_ones++;
        } else {
            max_len = std::max(max_len, left_ones + right_ones);
            left_ones = right_ones;
            right_ones = 0;
            zero_index = i;
        }
    }
    max_len = std::max(max_len, left_ones + right_ones);
    
    if (zero_index == -1)
    {
       max_len = n -1;
    }
   
    std::cout << max_len << std::endl;
    return 0;
}