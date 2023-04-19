#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

typedef struct Vertex{
    ll id;
} vertex;

typedef struct Edge{
    ll id;
    vertex *v1, *v2;
} edge;

typedef struct Graph{
    ll n, m;
    vector<vertex*> vertices;
    vector<edge*> edges;
} graph;

