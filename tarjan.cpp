#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

vector<vector<ll>> g;
vector<ll> low, d;
ll timer = 0;
stack<ll> s;
vector<vector<ll>> components;

void dfs(int curr, int parent) {
    if(d[curr] != -1) return;
    low[curr] = d[curr] = timer++;
    s.push(curr);
    for (int i = 0; i < g[curr].size(); i++) {
        int next = g[curr][i];
        if (d[next] == -1) {
            dfs(next,curr);
            low[curr] = min(low[curr], low[next]);
            if (low[next] >= d[curr]) {
                vector<ll> component;
                int node;
                do {
                    node = s.top();
                    s.pop();
                    component.push_back(node);
                } while(node != next);
                component.push_back(curr);
                components.push_back(component);
            }
        } else if(next != parent) {
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
        g[v].push_back(u);
    }
    low.resize(n);
    d.resize(n, -1);
    for (int i = 0; i < n; i++) {
        if (d[i] == -1) {
            dfs(i,-1);
        }
    }
    sort(components.begin(), components.end(), [](vector<ll> a, vector<ll> b) {
        return a[0] < b[0];
    });
    for (int i = 0; i < components.size(); i++) {
        sort(components[i].begin(), components[i].end());
        for (int j = 0; j < components[i].size(); j++) {
            cout << components[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}