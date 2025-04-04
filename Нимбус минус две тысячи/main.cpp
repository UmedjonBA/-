#include <iostream>
#include <vector>


int main(){
  int N;
  std::cin >> N;
    std::vector<int> speeds(N),l_min(N),r_min(N);
    for(int i = 0; i < N; ++i){
      std::cin >> speeds[i];
      if(i == 0) l_min[i] = speeds[i];
      else if(l_min[i-1] < speeds[i]) l_min[i] = l_min[i - 1];
      else l_min[i] = speeds[i];
     }
    for(int i = N - 1; i >= 0; --i)
    {
      if(i == N - 1) r_min[i] = speeds[i];
      else if(r_min[i+1] < speeds[i]) r_min[i] = r_min[i + 1];
      else r_min[i] = speeds[i];
    }
    
    // for(int i = 0; i < N; ++i) std::cout<<speeds[i]<<"  ";
    // std::cout<<"\n";
    // for(int i = 0; i < N; ++i) std::cout<<l_min[i]<<"  ";
    // std::cout<<"\n";
    // for(int i = 0; i < N; ++i) std::cout<<r_min[i]<<"  ";
    // std::cout<<"\n";
    
    int Q;
    std::cin >> Q;
    std::vector<int> res(Q);
    for(int i = 0; i < Q; ++i)
    {
        int l,r;
        std::cin>>l>>r;
        res[i] = std::min(l_min[l-1],r_min[r-1]);
    }
    
    for(int i = 0; i < Q; ++i)
    {
     std::cout<< res[i]<<std::endl;
    }
    return 0;
}
