#ifndef TARJAN_VISHKIN_PARALLEL_H
#define TARJAN_VISHKIN_PARALLEL_H

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

struct Edge;
struct Vertex;
struct Graph;

struct Vertex {
    ll id;
    vector<Edge*> edges;
};

struct Edge {
    Vertex* v1;
    Vertex* v2;
};

struct Graph {
    ll n, m;
    vector<Vertex*> vertices;
    vector<Edge*> edges;
};

edge* reverseEdge(edge* e);
bool cmp(edge* a, edge* b);
bool cmp2(edge* a, edge* b);
bool findEdgeById(const vector<Edge*>& edges, ll id);
ll LCA(vector<ll>& level, vector<ll>& parent, ll u, ll v);
void bfs(Graph* g, Graph* t, Graph* nt, vector<ll>& parent, vector<ll>& level);
void findConnectedComponents(Graph* g, vector<vector<ll>>& component);
void euler_tour(Graph* t, vector<ll>& succ);

#endif // TARJAN_VISHKIN_H