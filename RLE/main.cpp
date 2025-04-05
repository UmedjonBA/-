#include <iostream>
#include <string>

int main() {
  std::string s;
  std::cin >> s;

  std::string result = "";
  int count = 1;
  char current_char = s[0];

  for (size_t i = 1; i < s.length(); ++i) {
    if (s[i] == current_char) {
      count++;
    } else {
      result += current_char;
      if (count > 1) {
        result += std::to_string(count);
      }
      current_char = s[i];
      count = 1;
    }
  }
  result += current_char;
  if (count > 1) {
    result += std::to_string(count);
  }
    
    std::cout << result.length() << std::endl;
    std::cout << result << std::endl;
    
  return 0;
}