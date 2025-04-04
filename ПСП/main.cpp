#include <iostream>
#include <vector>

struct Stack {
  std::vector<char> stack;
  
  void Push(char symbol) { stack.push_back(symbol); }

  void Pop() { stack.pop_back(); }

  char Get() const { return stack.back(); }

  size_t Size() const { return stack.size(); }
};

bool IsPSP(const std::string& str) {
  if (str.empty())
  {
    return true;
  }
  Stack stack;

  for (size_t i = 0; i < str.size(); ++i)
  {
    if (stack.Size() == 0)
    {
      if (str[i] != '(' && str[i] != '{' && str[i] != '[') 
      {
        return false;
      }
      else 
      {
        stack.Push(str[i]);
        continue;
      }
    }
    else if (stack.Get() == '(' && str[i] != ')' ||
    		 stack.Get() == '[' && str[i] != ']' ||
             stack.Get() == '{' && str[i] != '}')
    {
      stack.Push(str[i]);
    }
    else
    {
      stack.Pop();
    }
  }

  return stack.Size() == 0;
}

int main() {
  std::string str;
  std::cin>>str;
  
  if (IsPSP(str)) {
    std::cout << "YES\n";
    return 0;
  }
  std::cout << "NO\n";
  return 0;
}
