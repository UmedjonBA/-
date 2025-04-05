#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {

    int n,m;cin>>n>>m;
    vector<int> x(n);
    for(int i=0;i<n;i++) cin>>x[i];

    vector<vector<int>> cost(n,vector<int>(n,0));
    for (int l=0; l<n; l++){
        for (int r=l; r<n; r++){
            int mid = (l+r)/2;
            int median = x[mid];
            int c=0;
            for(int t=l; t<=r; t++)
                c += abs(x[t]-median);
            cost[l][r]=c;
        }
    }

    const int INF=1000000000;
    vector<vector<int>> dp(n+1,vector<int>(m+1,INF));
    vector<vector<int>> par(n+1,vector<int>(m+1,-1));
    dp[0][0]=0;

    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            for(int k=0;k<i;k++){
                int val = dp[k][j-1]+cost[k][i-1];
                if(val<dp[i][j]){
                    dp[i][j]=val;
                    par[i][j]=k;
                }
            }
        }
    }

    cout<<dp[n][m]<<'\n';

    vector<int> centers;
    {
        int cur_n=n, cur_m=m;
        while(cur_m>0){
            int prev_n=par[cur_n][cur_m];
            int mid=(prev_n+(cur_n-1))/2;
            centers.push_back(x[mid]);
            cur_n=prev_n;
            cur_m--;
        }
        reverse(centers.begin(),centers.end());
    }

    for (auto c:centers) cout<<c<<" ";
    cout<<"\n";
    return 0;
}
