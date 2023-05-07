#include <bits/stdc++.h>
#include <omp.h>
#include "utils.h"

using namespace std;

double tarjan_vishkin_uf(graph *g){
    graph *t = new graph(), *nt = new graph();
    t->n = g->n;
    nt->n = g->n;
    vector<ll> pre, succ, low, parent(g->n, -1), level(g->n, -1);

    auto start = chrono::high_resolution_clock::now();

    bfs(g, t, nt, parent, level);
    sort(g->edges.begin(), g->edges.end(), cmp);
    sort(t->edges.begin(), t->edges.end(), cmp);
    sort(nt->edges.begin(), nt->edges.end(), cmp);

    t->m = t->edges.size();
    nt->m = nt->edges.size();

    // for(ll i = 0; i < t->m; i++)
    //     cout << t->edges[i]->v1->id << " " << t->edges[i]->v2->id << endl;

    euler_tour(t, succ);

    preOrderVertices(t, pre, succ);

    find_low(t, nt, low, level);

    graph *aux = auxillary_graph_uf(g, t, nt, low, level, parent, pre);
    // cout << aux->edges.size() << endl;

    vector<vector<ll>> connected;
    findConnectedComponents(aux, connected);

    vector<set<ll>> biconnected;
    remap_aux_graph(t, biconnected, connected, parent);

    while(biconnected.size() > 0 && biconnected[biconnected.size() - 1].size() == 0)
        biconnected.pop_back();

    auto end = chrono::high_resolution_clock::now();

    // cout << t->n << " " << t->m << endl;
    // cout << nt->n << " " << nt->m << endl;
    // cout << aux->n << " " << aux->m << endl;
    // for (set<ll> s : biconnected) {
    //     for (ll i : s)
    //         cout << i << " ";
    //     cout << endl;
    // }

    // cout << "UF Sequential Time: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() / 1e9 << "s" << endl;
    destroyGraph(t);
    destroyGraph(nt);
    return (chrono::duration_cast<chrono::nanoseconds>(end - start).count())/ 1e9;
}