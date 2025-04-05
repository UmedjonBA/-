#include <iostream>
#include <vector>
using namespace std;

int main(){
    int n,k;
    cin>>n>>k;
    if(k==0){
        if(n==1) cout<<0<<"\n"; else cout<<-1<<"\n";
        return 0;
    }
    if(n==1){cout<<0<<"\n";return 0;}
    vector<long long> dp(k+1,0);
    int t=0;
    while(true){
        t++;
        for (int j=k;j>=1;j--) dp[j]=dp[j-1]+dp[j]+1;
        if (dp[k]+1>=n) {
            cout<<t<<"\n";
            return 0;
        }
        if(t>n) {
            cout<<-1<<"\n";
            return 0;
        }
    }
}
