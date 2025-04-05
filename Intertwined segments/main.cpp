#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include <functional>
#include <cassert>

using namespace std;

struct Fenwick {
    int n;
    vector<int> fenw;
    Fenwick(int n):n(n),fenw(n+1,0){}
    void update(int i,int val=1){for(;i<=n;i+=(i&(-i))) fenw[i]+=val;}
    int query(int i){int s=0;for(;i>0;i-=(i&(-i))) s+=fenw[i];return s;}
};

struct PairHash {
    size_t operator()(const pair<int,int>& p) const {
        auto h1 = std::hash<int>()(p.first);
        auto h2 = std::hash<int>()(p.second);
        return h1 ^ (h2 + 0x9e3779b97f4a7c16ULL + (h1<<6) + (h1>>2));
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin>>n;
    vector<pair<int,int>> intervals(n);
    for (int i=0; i<n; i++) cin>>intervals[i].first>>intervals[i].second;
    sort(intervals.begin(), intervals.end(), [](auto &a, auto &b){
        if(a.first==b.first) return a.second>b.second;
        return a.first<b.first;
    });
    vector<int> rv(n);
    for (int i=0; i<n; i++) rv[i]=intervals[i].second;
    sort(rv.begin(), rv.end());
    rv.erase(unique(rv.begin(), rv.end()), rv.end());
    auto getIdx=[&](int x){return (int)(lower_bound(rv.begin(),rv.end(),x)-rv.begin())+1;};
    Fenwick fenw((int)rv.size());
    unordered_map<pair<int,int>,int,PairHash> same_count;
    same_count.reserve(n*2);
    same_count.max_load_factor(0.7);
    long long res=0;
    for (auto &it: intervals) {
        int l=it.first,r=it.second;
        int r_idx=getIdx(r);
        int total=fenw.query((int)rv.size());
        int geq=total-fenw.query(r_idx-1);
        int sc=0;
        auto itr=same_count.find(it);
        if(itr!=same_count.end()) sc=itr->second;
        int pairs=geq-sc;
        res+=pairs;
        fenw.update(r_idx);
        same_count[it]=sc+1;
    }
    cout<<res<<"\n";
}
