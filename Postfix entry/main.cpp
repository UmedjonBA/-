#include <iostream>
#include <vector>
#include <sstream>
#include <string>

struct Stack {
    std::vector<long long> stack;
  
    void Push(long long symbol) { stack.push_back(symbol); }

    void Pop() { stack.pop_back(); }

    long long Get() const { return stack.back(); }

    size_t Size() const { return stack.size(); }
};


long long Сalculate(std::string& str)
{
    Stack st;
    long long res;
    if(str.size() == 0)
    {
        return 0;
    }
    for(int i = 0; i < str.size(); ++i)
    {
        if(str[i] > 47 && str[i] < 58)
        {
            st.Push(static_cast<long long>(str[i] - 48));
        }
        else if(str[i] == '+')
        {
            res = st.Get();
            st.Pop();
            res = st.Get() + res;
            st.Pop();
            st.Push(res);
        }
        else if(str[i] == '-')
        {
            res = st.Get();
            st.Pop();
            res = st.Get() - res;
            st.Pop();
            st.Push(res);
        }
        else if(str[i] == '*')
        {
            res = st.Get();
            st.Pop();
            res = st.Get() * res;
            st.Pop();
            st.Push(res);
        }
    }
    return st.Get();
}

int main() {
    std::string str;
    getline(std::cin, str);
    std::cout<< Сalculate(str);
  return 0;
}
