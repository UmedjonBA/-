#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

bool areMapsEqual(const std::unordered_map<char, int>& map1, const std::unordered_map<char, int>& map2) {
    if (map1.size() != map2.size()) {
        return false;
    }
    for (const auto& pair : map1) {
        if (map2.find(pair.first) == map2.end() || map2.at(pair.first) != pair.second) {
            return false;
        }
    }
    return true;
}

int main() {
    std::string p, t;
    std::cin >> p >> t;

    if (p.length() > t.length()) {
        std::cout << "No" << std::endl;
        return 0;
    }

    std::unordered_map<char, int> p_freq;
    for (char c : p) {
        p_freq[c]++;
    }

    std::unordered_map<char, int> t_freq;
    for (size_t i = 0; i < p.length(); ++i) {
        t_freq[t[i]]++;
    }

    if (areMapsEqual(p_freq, t_freq)) {
       std::cout << "Yes" << std::endl;
        return 0;
    }

    for (size_t i = p.length(); i < t.length(); ++i) {
        t_freq[t[i]]++;
        t_freq[t[i - p.length()]]--;
         if(t_freq[t[i - p.length()]] == 0){
            t_freq.erase(t[i - p.length()]);
        }
        if (areMapsEqual(p_freq, t_freq)) {
            std::cout << "Yes" << std::endl;
            return 0;
        }
    }

    std::cout << "No" << std::endl;
    return 0;
}