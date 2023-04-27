#include <iostream>
#include <fstream>
#include "tarjan-vishkin.h"
#include "tarjan-vishkin-uf.h"
#include "tarjan.h"
#include "tarjan-vishkin-parallel.h"
#include "tv-parallel-union-find.h"

using namespace std;

int main(int argc, char **argv)
{

    if (argc < 2)
    {
        cout << "Usage: ./main <graph.txt>" << endl;
        return 0;
    }

    // prepare graph and other required data structures
    ifstream cin(argv[1]);

    graph *g = new graph();
    cin >> g->n >> g->m;
    for (int i = 0; i < g->n; i++)
    {
        vertex *v = new vertex();
        v->id = i;
        g->vertices.push_back(v);
    }
    for (int i = 0; i < 2 * g->m; i += 2)
    {
        edge *e = new edge();
        ll u, v;
        cin >> u >> v;
        e->id = i;
        e->v1 = g->vertices[u];
        e->v2 = g->vertices[v];
        g->vertices[u]->edges.push_back(e);
        g->vertices[v]->edges.push_back(e);
        g->edges.push_back(e);
        g->edges.push_back(reverseEdge(e, i + 1));
    }
    g->m *= 2;
    graph *t = new graph(), *nt = new graph();
    t->n = g->n;
    nt->n = g->n;
    vector<ll> pre, succ, low, parent(g->n, -1), level(g->n, -1);

    // run Tarjan-Vishkin
    tarjan_vishkin(g, t, nt, pre, succ, low, parent, level);
}