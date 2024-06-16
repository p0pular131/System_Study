#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
 
ll gcd(ll a, ll b) { for (; b; a %= b, swap(a, b)); return a; }
 
const int SIZE = 10001;
 
int v, e;
vector <int> graph[SIZE];
 
vector<vector<pii> > BCC;
 
int DFS_cnt = 1;
int DFS_num[SIZE];
int DFS_min[SIZE];
 
vector<pii> stk;
 
void DFS(int v, int p)
{
    DFS_num[v] = DFS_min[v] = DFS_cnt++;
    for (int nv : graph[v])
    {
        if (nv == p) continue;
        if (DFS_num[v] > DFS_num[nv]) stk.push_back({ v, nv });
        // 아직 방문하지 않은 간선을 스택에 넣는다.
 
        if (DFS_num[nv])
        {
            // 역방향 간선
            DFS_min[v] = min(DFS_min[v], DFS_num[nv]);
        }
        else
        {
            // 순방향 간선
            DFS(nv, v);
            DFS_min[v] = min(DFS_min[v], DFS_min[nv]);
            if (DFS_min[nv] >= DFS_num[v])
            {
                BCC.emplace_back();
                while (true)
                {
                    pii e = stk.back(); stk.pop_back();
                    BCC.back().push_back(e);
 
                    if (e == pii(v, nv)) break;
                }
            }
        }
    }
}
 
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);
 
    cin >> v >> e;
    for (int i = 0; i < e; i++)
    {
        int a, b; cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
 
    for (int i = 1; i <= v; i++)
    {
        if (!DFS_num[i]) DFS(i, 0);
    }
 
    cout << BCC.size() << '\n';
    for (auto& it : BCC)
    {
        for (pii e : it) cout << "(" << e.first << "," << e.second << ") ";
        cout << "\n";
    }
}
