#include <iostream>
#include <vector>
#include <algorithm>


void sort(std::vector<std::pair<int,int>>& arr, int l, int r) {
    if (l < r) {
       std::pair<int,int> pivot = arr[(l + r) / 2];
        int i = l, j = r;
        while (i <= j) {
            while (arr[i] < pivot) i++;
            while (arr[j] > pivot) j--;
            if (i <= j) {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
        sort(arr, l, j);
        sort(arr, i, r);
    }
}


int main()
{
    int n;
    std::cin>>n;
    std::vector<std::pair<int,int>> a(n),res;
    for(int i = 0; i< n; ++i) 
    {
        std::cin>>a[i].first >> a[i].second;
    }
    sort(a,0,n-1);
    // quickSort(a,0,n-1);
    // std::sort(a.begin(), a.end());
    int f = a[0].first, s = a[0].second; 
    std::cout<<std::endl;
    for(int i = 1; i< n; ++i) 
    {
        if(a[i].first <= s)
        {
            if(a[i].second > s) s = a[i].second;
        }
        else if(f < a[i].first)
        {
            //std::cout<<f<<" "<<s<<std::endl;
            res.push_back(std::make_pair(f,s));
            f = a[i].first;
            s = a[i].second;
        }
    }
    res.push_back(std::make_pair(f,s));
    std::cout<<res.size()<<std::endl;
    for(int i = 0; i < res.size(); ++i)
    {
        std::cout<<res[i].first<<' '<<res[i].second<<std::endl;
    }
    return 0;
}