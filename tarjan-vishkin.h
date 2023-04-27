#ifndef TARJAN_VISHKIN_H
#define TARJAN_VISHKIN_H

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct Edge;
struct Vertex;
struct Graph;

typedef struct Vertex {
    ll id;
    vector<Edge*> edges;
} vertex;

typedef struct Edge {
    Vertex* v1;
    Vertex* v2;
} edge;

typedef struct Graph {
    ll n, m;
    vector<Vertex*> vertices;
    vector<Edge*> edges;
} graph;

edge* reverseEdge(edge* e);
bool cmp(edge* a, edge* b);
bool cmp2(edge* a, edge* b);
ll LCA(vector<ll>& level, vector<ll>& parent, ll u, ll v);
void bfs(graph* g, graph* t, graph* nt, vector<ll>& parent, vector<ll>& level);
void findConnectedComponents(graph* g, vector<vector<ll>>& component);
void euler_tour(graph* t, vector<ll>& succ);
void tarjan_vishkin(graph* g, graph* t, graph* nt, vector<ll>& pre, vector<ll>& succ, vector<ll>& low, vector<ll>& parent, vector<ll>&  level);
#endif // TARJAN_VISHKIN_H
