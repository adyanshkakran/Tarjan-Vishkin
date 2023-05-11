#include <bits/stdc++.h>
#include <omp.h>
#include "utils.h"

using namespace std;

double tarjan_vishkin(graph* g, bool edgeFlag = false)
{
    graph *t = new graph(), *nt = new graph();
    t->n = g->n;
    nt->n = g->n;
    vector<ll> pre, succ, low, parent(g->n, -1), level(g->n, -1);
    
    auto start = chrono::high_resolution_clock::now();

    bfs(g,t,nt, parent, level);
    sort(g->edges.begin(), g->edges.end(), cmp);
    sort(t->edges.begin(), t->edges.end(), cmp);
    sort(nt->edges.begin(), nt->edges.end(), cmp);

    t->m = t->edges.size();
    nt->m = nt->edges.size();

    euler_tour(t, succ);

    preOrderVertices(t, pre, succ);

    find_low(t, nt, low, level);

    // print pre
    for(int i = 0; i < pre.size(); i++)
        if(pre[i] == -1)
            cout << pre[i] << " ";
    cout << endl;


    graph* aux = auxillary_graph(g, t, nt, low, level, parent, pre);
    int aux_edges = aux->edges.size();

    vector<vector<ll>> connected;
    findConnectedComponents(aux, connected);

    vector<set<ll>> biconnected;
    remap_aux_graph(t, biconnected, connected, parent);

    while(biconnected.size() > 0 && biconnected[biconnected.size() - 1].size() == 0)
        biconnected.pop_back();

    auto end = chrono::high_resolution_clock::now();

    destroyGraph(t);
    destroyGraph(nt);
    destroyGraph(aux);

    if (edgeFlag)
    {
        return aux_edges;
    }
    
    return (chrono::duration_cast<chrono::nanoseconds>(end - start).count())/ 1e9;
}