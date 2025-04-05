#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    cin>>n;
    vector<pair<int,string>> M(n);
    for (int i=0;i<n;i++){string s;int d;cin>>s>>d;M[i]={d,s};}
    int m;cin>>m;
    vector<pair<int,string>> W(m);
    for (int i=0;i<m;i++){string s;int d;cin>>s>>d;W[i]={d,s};}
    sort(M.begin(),M.end());reverse(M.begin(),M.end());
    sort(W.begin(),W.end());reverse(W.begin(),W.end());
    const int INF=1000000000;
    static long long dp[2012][2012];
    static int pm[2012][2012], pw[2012][2012], pat[2012][2012];
    for (int i=0;i<=n;i++) for (int j=0;j<=m;j++) dp[i][j]=1LL<<50;
    dp[0][0]=0;
    for (int i=0;i<=n;i++){
        for (int j=0;j<=m;j++){
            if (dp[i][j]==(1LL<<50)) continue;
            if (i<n){
                {
                    long long cost=M[i].first;
                    if (dp[i+1][j]>dp[i][j]+cost){
                        dp[i+1][j]=dp[i][j]+cost;
                        pm[i+1][j]=1;pw[i+1][j]=0;pat[i+1][j]=1;
                    }
                }
                if (i+1<n){
                    long long cost=max((long long)M[i].first,(long long)M[i+1].first);
                    if (dp[i+2][j]>dp[i][j]+cost){
                        dp[i+2][j]=dp[i][j]+cost;
                        pm[i+2][j]=2;pw[i+2][j]=0;pat[i+2][j]=2;
                    }
                }
                if (j<m){
                    long long cost=max((long long)M[i].first,(long long)W[j].first);
                    if (dp[i+1][j+1]>dp[i][j]+cost){
                        dp[i+1][j+1]=dp[i][j]+cost;
                        pm[i+1][j+1]=1;pw[i+1][j+1]=1;pat[i+1][j+1]=3;
                    }
                }
                if (i+2<n){
                    long long cost=max({(long long)M[i].first,(long long)M[i+1].first});
                    if (j<m){
                        cost=max(cost,(long long)W[j].first);
                        if (dp[i+2][j+1]>dp[i][j]+cost){
                            dp[i+2][j+1]=dp[i][j]+cost;
                            pm[i+2][j+1]=2;pw[i+2][j+1]=1;pat[i+2][j+1]=4;
                        }
                    }
                }
                if (i+2<n){
                    long long cost=max({(long long)M[i].first,(long long)M[i+1].first,(long long)M[i+2].first});
                    if (dp[i+3][j]>dp[i][j]+cost){
                        dp[i+3][j]=dp[i][j]+cost;
                        pm[i+3][j]=3;pw[i+3][j]=0;pat[i+3][j]=5;
                    }
                }
                if (i+1<n && j+1<=m){
                    long long cost=max({(long long)M[i].first,(long long)M[i+1].first,(long long)W[j].first});
                    if (dp[i+2][j+1]>dp[i][j]+cost){
                        dp[i+2][j+1]=dp[i][j]+cost;
                        pm[i+2][j+1]=2;pw[i+2][j+1]=1;pat[i+2][j+1]=6;
                    }
                }
                if (j+2<=m){
                    long long cost=max({(long long)M[i].first,(long long)W[j].first,(long long)W[j+1].first});
                    if (dp[i+1][j+2]>dp[i][j]+cost){
                        dp[i+1][j+2]=dp[i][j]+cost;
                        pm[i+1][j+2]=1;pw[i+1][j+2]=2;pat[i+1][j+2]=7;
                    }
                }
                if (i+1<n && j+2<=m){
                    long long cost=max({(long long)M[i].first,(long long)M[i+1].first,(long long)W[j].first,(long long)W[j+1].first});
                    if (dp[i+2][j+2]>dp[i][j]+cost){
                        dp[i+2][j+2]=dp[i][j]+cost;
                        pm[i+2][j+2]=2;pw[i+2][j+2]=2;pat[i+2][j+2]=8;
                    }
                }
                if (j+3<=m){
                    long long cost=max({(long long)M[i].first,(long long)W[j].first,(long long)W[j+1].first,(long long)W[j+2].first});
                    if (dp[i+1][j+3]>dp[i][j]+cost){
                        dp[i+1][j+3]=dp[i][j]+cost;
                        pm[i+1][j+3]=1;pw[i+1][j+3]=3;pat[i+1][j+3]=9;
                    }
                }
                if (i+3<n){
                    long long cost=max({(long long)M[i].first,(long long)M[i+1].first,(long long)M[i+2].first,(long long)M[i+3].first});
                    if (dp[i+4][j]>dp[i][j]+cost){
                        dp[i+4][j]=dp[i][j]+cost;
                        pm[i+4][j]=4;pw[i+4][j]=0;pat[i+4][j]=10;
                    }
                }
                if (i+3<n && j<m){
                    long long cost=max({(long long)M[i].first,(long long)M[i+1].first,(long long)M[i+2].first,(long long)W[j].first});
                    if (dp[i+3][j+1]>dp[i][j]+cost){
                        dp[i+3][j+1]=dp[i][j]+cost;
                        pm[i+3][j+1]=3;pw[i+3][j+1]=1;pat[i+3][j+1]=11;
                    }
                }
            }
        }
    }
    long long res=dp[n][m];
    cout<<res<<"\n";
    vector<vector<string>> ans;
    {
        int x=n,y=m;
        while (x>0){
            int P=pat[x][y];
            int mm=pm[x][y],ww=pw[x][y];
            vector<string> g;
            int xx=x-mm,yy=y-ww;
            int di=xx, dj=yy;
            if (P==1){
                g.push_back(M[xx].second);
            } else if (P==2){
                g.push_back(M[xx].second);g.push_back(M[xx+1].second);
            } else if (P==3){
                g.push_back(M[xx].second);g.push_back(W[dj].second);
            } else if (P==4){
                g.push_back(M[xx].second);g.push_back(M[xx+1].second);g.push_back(W[dj].second);
            } else if (P==5){
                g.push_back(M[xx].second);g.push_back(M[xx+1].second);g.push_back(M[xx+2].second);
            } else if (P==6){
                g.push_back(M[xx].second);g.push_back(M[xx+1].second);g.push_back(W[dj].second);
            } else if (P==7){
                g.push_back(M[xx].second);g.push_back(W[dj].second);g.push_back(W[dj+1].second);
            } else if (P==8){
                g.push_back(M[xx].second);g.push_back(M[xx+1].second);g.push_back(W[dj].second);g.push_back(W[dj+1].second);
            } else if (P==9){
                g.push_back(M[xx].second);g.push_back(W[dj].second);g.push_back(W[dj+1].second);g.push_back(W[dj+2].second);
            } else if (P==10){
                g.push_back(M[xx].second);g.push_back(M[xx+1].second);g.push_back(M[xx+2].second);g.push_back(M[xx+3].second);
            } else if (P==11){
                g.push_back(M[xx].second);g.push_back(M[xx+1].second);g.push_back(M[xx+2].second);g.push_back(W[dj].second);
            }
            ans.push_back(g);
            x=xx;y=dj;
        }
    }
    cout<<(int)ans.size()<<"\n";
    for (int i=0;i<(int)ans.size();i++){
        cout<<"Taxi "<<i+1<<": ";
        int sz=(int)ans[i].size();
        if (sz==1) {
            cout<<ans[i][0]<<".\n";
        } else if (sz==2) {
            cout<<ans[i][0]<<" and "<<ans[i][1]<<".\n";
        } else {
            for (int j=0;j<sz;j++){
                if (j==sz-1) cout<<"and "<<ans[i][j]<<".\n";
                else {
                    cout<<ans[i][j];
                    if (j<sz-2) cout<<", ";
                    else cout<<" ";
                }
            }
        }
    }
    return 0;
}
