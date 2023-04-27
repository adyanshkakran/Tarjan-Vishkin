#ifndef TV_PARALLEL_UNION_FIND_H
#define TV_PARALLEL_UNION_FIND_H

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

edge* reverseEdge(edge* e);
bool cmp(edge* a, edge* b);
bool cmp2(edge* a, edge* b);
bool findEdgeById(const vector<Edge*>& edges, ll id);
ll LCA(vector<ll>& level, vector<ll>& parent, ll u, ll v);
void bfs(Graph* g, Graph* t, Graph* nt, vector<ll>& parent, vector<ll>& level);
void findConnectedComponents(Graph* g, vector<vector<ll>>& component);
void euler_tour(Graph* t, vector<ll>& succ);

#endif // TV_PARALLEL_UNION_FIND_H