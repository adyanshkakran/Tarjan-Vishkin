#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<vector<ll>> g;
vector<bool> vis;
vector<ll> low, d, mlow;
ll timer = 0;
bool rootArt = false;

void dfs(int curr, int root) {
    vis[curr] = true;
    low[curr] = d[curr] = timer++;
    mlow[curr] = -1;
    for (int i = 0; i < g[curr].size(); i++) {
        int next = g[curr][i];
        if (!vis[next]) {
            if(curr == root && i != 0){
                rootArt = true;
            }
            dfs(next,root);
            low[curr] = min(low[curr], low[next]);
            mlow[curr] = max(mlow[curr], low[next]);
        } else {
            low[curr] = min(low[curr], d[next]);
        }
    }
}

int main() {
    ifstream cin("datasets/graph1.txt");
    int n, m;
    cin >> n >> m;
    g.resize(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
    }
    vis.resize(n);
    low.resize(n);
    mlow.resize(n);
    d.resize(n);
    set<ll> roots;
    vector<ll> articulation;
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            roots.insert(i);
            dfs(i,i);
            if(rootArt){
                articulation.push_back(i);
                rootArt = false;
            }
        }
    }
    for(int i = 0; i < n; i++) {
        if(roots.find(i) == roots.end() && mlow[i] != -1 && d[i] <= mlow[i]) {
            articulation.push_back(i);
        }
    }
    for (int i = 0; i < articulation.size(); i++) {
        cout << articulation[i] << " ";
    }
    cout << endl;
}