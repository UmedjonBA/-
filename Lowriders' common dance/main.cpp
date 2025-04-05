#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main(){
    string s,t;cin>>s>>t;
    int n=(int)s.size(),m=(int)t.size();
    vector<vector<int>> dp(n+1,vector<int>(m+1,0));
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            if(s[i-1]==t[j-1])dp[i][j]=dp[i-1][j-1]+1;
            else dp[i][j]=max(dp[i-1][j],dp[i][j-1]);
        }
    }
    int len=dp[n][m];
    cout<<len<<"\n";
    if(len==0)return 0;
    vector<int> idx_s,idx_t;idx_s.reserve(len);idx_t.reserve(len);
    int i=n,j=m;
    while(i>0 && j>0){
        if(s[i-1]==t[j-1]){
            idx_s.push_back(i);
            idx_t.push_back(j);
            i--;j--;
        }else if(dp[i-1][j]>dp[i][j-1])i--;
        else j--;
    }
    reverse(idx_s.begin(),idx_s.end());
    reverse(idx_t.begin(),idx_t.end());
    for(auto &x:idx_s)cout<<x<<" ";cout<<"\n";
    for(auto &x:idx_t)cout<<x<<" ";cout<<"\n";
}
