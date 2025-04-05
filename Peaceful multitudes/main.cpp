#include <bits/stdc++.h>
using namespace std;

static long long memo[2001][2001];
static bool used[2001][2001];

long long dp(int rem, int minE) {
    if (rem == 0) return 1;
    if (minE > rem) return 0;
    if (used[rem][minE]) return memo[rem][minE];
    used[rem][minE] = true;
    long long &res = memo[rem][minE];
    res = 0;
    for (int e = minE; e <= rem; e++) {
        // Следующий минимальный элемент будет не меньше 2*e
        res += dp(rem - e, 2 * e);
    }
    return res;
}

int main() {
    int n;
    cin >> n;
    cout << dp(n, 1) << "\n";

    return 0;
}
