#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n; cin >> n;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;
    vector<long long> b(n);
    for (int i = 0; i < n; i++) b[i] = -a[i];
    vector<long long> d; d.reserve(n);
    vector<int> pos; pos.reserve(n);
    vector<int> p(n, -1);
    for (int i = 0; i < n; i++) {
        int j = (int)(upper_bound(d.begin(), d.end(), b[i]) - d.begin());
        if (j == (int)d.size()) {
            d.push_back(b[i]);
            pos.push_back(i);
        } else {
            d[j] = b[i];
            pos[j] = i;
        }
        if (j > 0) p[i] = pos[j - 1];
    }
    int length = (int)d.size();
    cout << length << "\n";
    vector<int> ans; ans.reserve(length);
    for (int cur = pos.back(); cur != -1; cur = p[cur]) ans.push_back(cur + 1);
    reverse(ans.begin(), ans.end());
    for (auto &x : ans) cout << x << "\n";
}
