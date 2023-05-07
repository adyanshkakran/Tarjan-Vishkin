#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

double tarjan_vishkin_parallel_uf(graph* g) {
    graph *t = new graph(), *nt = new graph();
    t->n = g->n;
    nt->n = g->n;
    vector<ll> pre, succ, low, parent(g->n, -1), level(g->n, -1);

    auto start = chrono::high_resolution_clock::now();

    bfs(g,t,nt, parent, level);
    sort(g->edges.begin(), g->edges.end(), cmp);
    sort(t->edges.begin(), t->edges.end(), cmp);
    sort(nt->edges.begin(), nt->edges.end(), cmp);

    // print the edges of the tree
    // cout << "tree edges\n";
    // for (size_t i = 0; i < t->edges.size(); i++) {
    //     cout << t->edges[i]->v1->id << " " << t->edges[i]->v2->id << " " << t->edges[i]->id << endl;
    // }

    // // print the edges of the non-tree
    // cout << "non-tree edges\n";
    // for (size_t i = 0; i < nt->edges.size(); i++) {
    //     cout << nt->edges[i]->v1->id << " " << nt->edges[i]->v2->id << " " << nt->edges[i]->id << endl;
    // }

    // cout << "global\n";
    // for (size_t i = 0; i < g->edges.size(); i++) {
    //     cout << g->edges[i]->v1->id << " " << g->edges[i]->v2->id << " " << g->edges[i]->id << endl;
    // }
 
    t->m = t->edges.size();
    nt->m = nt->edges.size();

    euler_tour_parallel(t, succ);

    preOrderVertices(t, pre, succ);
 
    find_low_parallel(t, nt, low, level);

    graph* aux = auxillary_graph_parallel_uf(g, t, nt, low, level, parent, pre);

    // print aux graph edges
    // cout << "aux edges\n";
    // for (size_t i = 0; i < aux->edges.size(); i++) {
    //     cout << aux->edges[i]->v1->id << " " << aux->edges[i]->v2->id << endl;
    // }

    vector<vector<ll>> connected;
    findConnectedComponents(aux, connected);

    // cout << connected.size() << endl;

    // print components
    // cout << "components\n";
    // for (size_t i = 0; i < connected.size(); i++) {
    //     for (size_t j = 0; j < connected[i].size(); j++) {
    //         cout << connected[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    vector<set<ll>> biconnected;
    remap_aux_graph_parallel(t, biconnected, connected, parent);

    while(biconnected.size() > 0 && biconnected[biconnected.size() - 1].size() == 0)
        biconnected.pop_back();

    auto end = chrono::high_resolution_clock::now();


    // cout << t->n << " " << t->m << endl;
    // cout << nt->n << " " << nt->m << endl;
    // cout << aux->n << " " << aux->m << endl

    // int count = 0;
    // for (set<ll> s : biconnected) {
    //     // cout << count << ": ";
    //     // for (ll i : s)
    //     //     cout << i << " ";
    //     // cout << endl;
    //     count++;
    // }
    // cout << count << endl;

    // cout << "Parallel time: " << (chrono::duration_cast<chrono::nanoseconds>(end - start).count())/ 1e9 << "s" << endl;
    destroyGraph(t);
    destroyGraph(nt);
    return (chrono::duration_cast<chrono::nanoseconds>(end - start).count())/ 1e9;
}