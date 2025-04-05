#include <iostream>
#include <string>

int main() {
    std::string s;
    std::cin >> s;

    int max_len = 0;
    int open_count = 0;
    int balance = 0;

    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == '(') {
            balance++;
        } else {
            balance--;
        }

        if (balance < 0) {
            balance = 0;
            open_count = 0;
        } else {
            open_count++;
            if (balance == 0){
                max_len = std::max(max_len, open_count);
            }
        }
    }
    
    balance = 0;
    open_count = 0;

    for (int i = s.length() - 1; i >= 0; --i) {
        if (s[i] == ')') {
            balance++;
        } else {
            balance--;
        }

        if (balance < 0) {
            balance = 0;
            open_count = 0;
        } else {
             open_count++;
             if (balance == 0){
                 max_len = std::max(max_len, open_count);
             }
        }
    }
    
    std::cout << max_len << std::endl;
    return 0;
}