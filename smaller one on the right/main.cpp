#include <iostream>
#include <vector>


int main()
{
    int n;
    std::cin>>n;
    std::vector<int> a(n),ress(n);
    for(int i = 0; i< n; ++i) 
    {
        std::cin>>a[i];
    }
    for(int i = n - 1; i >= 0; --i)
    {
        int res = 0;
        for(int j = i; j < n; ++j)
        {
            if(a[i] > a[j]) ++res;
        }
        ress[i] = res;
    }
    for(int i = 0; i< n; ++i) 
    {
        std::cout<<ress[i]<<' ';
    }
    
    
    return 0;
}