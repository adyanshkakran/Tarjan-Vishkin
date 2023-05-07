#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

extern inline int PER_THREAD;
extern inline int THREADS;

typedef long long ll;

typedef struct Edge edge;
typedef struct Vertex vertex;
typedef struct Graph graph;

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
    double avgDegree;
};

class UnionFind {
public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }

    int find(int x) {
        if (parent[x] == x) {
            return x;
        } else {
            parent[x] = find(parent[x]);
            return parent[x];
        }
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) {
            return;
        }
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

void destroyGraph(Graph* g);
void destroyGraph2(Graph *g);
edge* reverseEdge(edge* e);
bool cmp(edge* a, edge* b);
bool cmp2(edge* a, edge* b);
ll LCA(vector<ll>& level, vector<ll>& parent, ll u, ll v);
void bfs(Graph* g, Graph* t, Graph* nt, vector<ll>& parent, vector<ll>& level);
void findConnectedComponents(Graph* g, vector<vector<ll>>& component);
bool findEdgeById(const vector<Edge*>& edges, ll id);
void euler_tour(Graph* t, vector<ll>& succ);
void euler_tour_parallel(Graph* t, vector<ll>& succ);
void preOrderVertices(graph* t, vector<ll>& pre, vector<ll>& succ);
void find_low(graph* t, graph* nt, vector<ll>& low, vector<ll>& level);
void find_low_parallel(graph* t, graph* nt, vector<ll>& low, vector<ll>& level);
graph* auxillary_graph_parallel(graph* g, graph* t, graph *nt,vector<ll>& low, vector<ll>& level, vector<ll>& parent, vector<ll>& pre);
void remap_aux_graph_parallel(graph* t, vector<set<ll>>& bi, vector<vector<ll>>& components, vector<ll>& parent);
graph* auxillary_graph(graph* g, graph* t, graph *nt,vector<ll>& low, vector<ll>& level, vector<ll>& parent, vector<ll>& pre);
graph *auxillary_graph_uf(graph *g, graph *t, graph *nt, vector<ll> &low, vector<ll> &level, vector<ll> &parent, vector<ll> &pre);
graph *auxillary_graph_parallel_uf(graph *g, graph *t, graph *nt, vector<ll> &low, vector<ll> &level, vector<ll> &parent, vector<ll> &pre);
void remap_aux_graph(graph* t, vector<set<ll>>& bi, vector<vector<ll>>& components, vector<ll>& parent);

#endif // UTILS_H