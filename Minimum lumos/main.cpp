#include <iostream>
#include <vector>
#include <algorithm>

void quickSort(std::vector<int> &vec, int low, int high) {

    if (low < high) 
    {

        int pivot = vec[high];

        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {

            if (vec[j] <= pivot) {
                i++;
                std::swap(vec[i], vec[j]);
            }
        }

        std::swap(vec[i + 1], vec[high]);
        quickSort(vec, low, i);
        quickSort(vec, i + 2, high);
    }
}

bool predecat(std::vector<int> &vec, int l, int k)
{
    long long max = vec[0] + l;
    --k;
    for(int i = 0; i < vec.size(); ++i)
    {
        if(vec[i] > max )
        {
            max = vec[i] + l; 
            --k;
        }
        if(0 > k) return false;
    }
    return true;
}

void minlumos()
{
    int n,k;
    std::cin>>n>>k;
    if(n == k)
    {
        std::cout<<0<<std::endl;
        return ;

    }
    std::vector<int> arr(n);
    for(int i=0;i<n;++i) std::cin>>arr[i];
    //quickSort(arr,0,arr.size()-1);
    std::sort(arr.begin(),arr.end());
    long long l=0,r = arr[arr.size() - 1] - arr[0];
    long long mid;
    
    while (l < r)
    {
        mid = (l+r) / 2;
        if(predecat(arr,mid,k)) r = mid;
        else l = mid + 1;
    }
    std::cout<<r<<std::endl;
}



int main()
{
    minlumos();
    return 0;
}