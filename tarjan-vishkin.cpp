#include <bits/stdc++.h>
#include <omp.h>
#include "utils.h"

using namespace std;

double tarjan_vishkin(graph *g, bool edgeFlag = false)
{
    graph *t = new graph(), *nt = new graph();
    t->n = g->n;
    nt->n = g->n;
    vector<ll> pre, succ, low, parent(g->n, -1), level(g->n, -1);

    auto start = chrono::high_resolution_clock::now();

    vector<vector<ll>> components;
    findConnectedComponents(g, components);

    for (int i = 0; i < components.size(); i++)
    {
        graph *subg = new graph();
        subg->n = components[i].size();
        subg->m = 0;

        for (int j = 0; j < components[i].size(); j++)
        {
            subg->vertices.push_back(g->vertices[components[i][j]]);
        }

        for (int j = 0; j < components[i].size(); j++)
        {
            for (int k = 0; k < g->vertices[components[i][j]]->edges.size(); k++)
            {
                if (find(components[i].begin(), components[i].end(), g->vertices[components[i][j]]->edges[k]->v2->id) != components[i].end())
                {
                    subg->edges.push_back(g->vertices[components[i][j]]->edges[k]);
                    subg->m++;
                }
            }
        }

        // print subgraph
        cout << "subgraph " << i << endl;
        for (int j = 0; j < subg->vertices.size(); j++)
        {
            cout << subg->vertices[j]->id << " ";
        }
        cout << endl;

        for (int j = 0; j < subg->edges.size(); j++)
        {
            cout << subg->edges[j]->v1->id << " " << subg->edges[j]->v2->id << endl;
        }

        continue;

        bfs(g, t, nt, parent, level);
        sort(g->edges.begin(), g->edges.end(), cmp);
        sort(t->edges.begin(), t->edges.end(), cmp);
        sort(nt->edges.begin(), nt->edges.end(), cmp);

        t->m = t->edges.size();
        nt->m = nt->edges.size();

#ifdef DEBUG
        // print tree and non-tree vertices & edges
        cout << "tree vertices\n";
        for (size_t i = 0; i < t->vertices.size(); i++)
        {
            cout << t->vertices[i]->id << " ";
        }
        cout << endl;

        cout << "tree edges\n";
        for (size_t i = 0; i < t->edges.size(); i++)
        {
            cout << t->edges[i]->v1->id << " " << t->edges[i]->v2->id << endl;
        }

        cout << "non-tree vertices\n";
        for (size_t i = 0; i < nt->vertices.size(); i++)
        {
            cout << nt->vertices[i]->id << " ";
        }
        cout << endl;

        cout << "non-tree edges\n";
        for (size_t i = 0; i < nt->edges.size(); i++)
        {
            cout << nt->edges[i]->v1->id << " " << nt->edges[i]->v2->id << endl;
        }

        cout << "level\n";
        for (size_t i = 0; i < level.size(); i++)
        {
            cout << level[i] << " ";
        }
        cout << endl;

        cout << "parent\n";
        for (size_t i = 0; i < parent.size(); i++)
        {
            cout << parent[i] << " ";
        }
        cout << endl;
#endif

        euler_tour(t, succ);

        preOrderVertices(t, pre, succ);

#ifdef DEBUG
        cout << "pre\n";
        for (size_t i = 0; i < pre.size(); i++)
        {
            cout << pre[i] << " ";
        }
        cout << endl;
#endif

        find_low(t, nt, low, level, pre, parent);

#ifdef DEBUG
        cout << "low\n";
        for (size_t i = 0; i < low.size(); i++)
        {
            cout << low[i] << " ";
        }
        cout << endl;
#endif

        graph *aux = auxillary_graph(g, t, nt, low, level, parent, pre);
        int aux_edges = aux->edges.size();

        vector<vector<ll>> connected;
        findConnectedComponents(aux, connected);

        vector<set<ll>> biconnected;
        remap_aux_graph(t, biconnected, connected, parent);

        while (biconnected.size() > 0 && biconnected[biconnected.size() - 1].size() == 0)
            biconnected.pop_back();

        auto end = chrono::high_resolution_clock::now();

        destroyGraph(t);
        destroyGraph(nt);
        destroyGraph(aux);

        if (edgeFlag)
        {
            return aux_edges;
        }

        cout << "Components(TV): " << biconnected.size() << endl;
        for (int i = 0; i < biconnected.size(); i++)
        {
            cout << "Component " << i << ": ";
            for (auto it = biconnected[i].begin(); it != biconnected[i].end(); it++)
                cout << *it << " ";
            cout << endl;
        }
    }
    return 0;
    // return (chrono::duration_cast<chrono::nanoseconds>(end - start).count()) / 1e9;
}