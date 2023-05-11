#include <bits/stdc++.h>
#include "tarjan.h"

using namespace std;
typedef long long ll;

ll timer = 0;
stack<ll> s;

void dfs(ll curr, ll parent, graph* g, vector<vector<ll>>& components, vector<ll>& low, vector<ll>& d) {
    if(d[curr] != -1) return;
    low[curr] = d[curr] = timer++;
    s.push(curr);
    for (int i = 0; i < g->vertices[curr]->edges.size(); i++) {
        int next = g->vertices[curr]->edges[i]->v1->id == curr ? g->vertices[curr]->edges[i]->v2->id : g->vertices[curr]->edges[i]->v1->id;
        if (d[next] == -1) {
            dfs(next,curr, g, components, low, d);
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

double tarjan(graph *g) {
    timer = 0;
    while(!s.empty()) s.pop();
    vector<ll> low, d;
    vector<vector<ll>> components;
    low.resize(g->n);
    d.resize(g->n, -1);
    vector<bool> done(g->n, false);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < g->n; i++) {
        if (d[i] == -1) {
            dfs(i, -1, g, components, low, d);
        }
    }

    for(int i = 0; i < components.size(); i++){
        sort(components[i].begin(), components[i].end());
        for(int j = 0; j < components[i].size(); j++)
            done[components[i][j]] = true;
    }
    
    for(int i = 0; i < g->n; i++)
        if(!done[i])
            components.push_back({i});

    auto end = chrono::high_resolution_clock::now();
    
    return (chrono::duration_cast<chrono::nanoseconds>(end - start).count())/ 1e9;
}