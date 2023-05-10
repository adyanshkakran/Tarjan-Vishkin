#include <bits/stdc++.h>
#include <omp.h>
#include "utils.h"

using namespace std;

double tarjan_vishkin(graph *g, bool edgeFlag = false)
{

    auto start = chrono::high_resolution_clock::now();

    vector<vector<ll>> components;
    findConnectedComponents(g, components);

    vector<set<ll>> biconnectedGlobal;

    for (int i = 0; i < components.size(); i++)
    {
        graph *subg = new graph();
        subg->n = components[i].size();
        subg->m = 0;

        for (int j = 0; j < components[i].size(); j++)
        {
            vertex *v = new vertex();
            v->id = j;
            subg->vertices.push_back(v);
        }

        // map graph id to subgraph id using vector of pair of graph id and subgraph id
        vector<ll> idMap(g->vertices.size(), -1);

        // reverse idMap to map subgraph id to graph id
        vector<ll> idMapRev(subg->vertices.size(), -1);

        for (int j = 0; j < subg->vertices.size(); j++)
        {
            idMap[components[i][j]] = j;    // idMap[graph id] = subgraph id
            idMapRev[j] = components[i][j]; // idMapRev[subgraph id] = graph id
        }

        // print idMap
        // cout << "idMap" << endl;
        // for (int j = 0; j < idMap.size(); j++)
        // {
        //     cout << j << " " << idMap[j] << endl;
        // }

        // print edges for every vertex in g

        // cout << "edges" << endl;
        // add edges to subgraph and map edge id to subgraph edge id
        for (int j = 0; j < components[i].size(); j++)
        {
            vertex *v = g->vertices[components[i][j]];
            for (int k = 0; k < v->edges.size(); k++)
            {
                edge *e = v->edges[k];
                // cout << v->id << ": " << e->v1->id << " " << e->v2->id << endl;
                // continue;
                edge *ee = new edge();
                ee->v1 = subg->vertices[idMap[e->v1->id]];
                ee->v2 = subg->vertices[idMap[e->v2->id]];
                subg->vertices[idMap[e->v1->id == v->id ? e->v2->id : e->v1->id]]->edges.push_back(ee);
                subg->edges.push_back(ee);
                subg->m++;
            }
        }

        // continue;

#ifdef DEBUG
        // print subgraph
        cout << "subgraph " << i << endl;
        for (int j = 0; j < subg->vertices.size(); j++)
        {
            cout << idMapRev[subg->vertices[j]->id] << ": ";
            for (int k = 0; k < subg->vertices[j]->edges.size(); k++)
            {
                cout << idMapRev[subg->vertices[j]->edges[k]->v1->id] << " " << idMapRev[subg->vertices[j]->edges[k]->v2->id] << " | ";
            }
            cout << endl;
        }
        cout << endl;
#endif

        // if subgraph is only one vertex, skip
        if (subg->n == 1)
        {
            // add to global biconnected components
            set<ll> s;
            s.insert(idMapRev[0]);
            biconnectedGlobal.push_back(s);
            continue;
        }

        graph *t = new graph(), *nt = new graph();
        t->n = subg->n;
        nt->n = subg->n;
        vector<ll> pre, succ, low, parent(subg->n, -1), level(subg->n, -1);

        bfs(subg, t, nt, parent, level);
        sort(subg->edges.begin(), subg->edges.end(), cmp);
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

        graph *aux = auxillary_graph(subg, t, nt, low, level, parent, pre);
        int aux_edges = aux->edges.size();

#ifdef DEBUG
        cout << "auxillary graph\n";
        for (size_t i = 0; i < aux->vertices.size(); i++)
        {
            cout << aux->vertices[i]->id << ": ";
            for (size_t j = 0; j < aux->vertices[i]->edges.size(); j++)
            {
                cout << aux->vertices[i]->edges[j]->v1->id << " " << aux->vertices[i]->edges[j]->v2->id << " | ";
            }
            cout << endl;
        }
        cout << endl;
#endif

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

        // add to global biconnected components
        for (int i = 0; i < biconnected.size(); i++)
        {
            set<ll> s;
            for (auto it = biconnected[i].begin(); it != biconnected[i].end(); it++)
            {
                // use reverse map to get original vertex id
                s.insert(idMapRev[*it]);
            }
            biconnectedGlobal.push_back(s);
        }
    }

    cout << "Components(TV): " << biconnectedGlobal.size() << endl;
    // for (int i = 0; i < biconnectedGlobal.size(); i++)
    // {
    //     cout << "Component " << i << ": ";
    //     for (auto it = biconnectedGlobal[i].begin(); it != biconnectedGlobal[i].end(); it++)
    //     {
    //         // use reverse map to get original vertex id
    //         cout << *it << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
    // return (chrono::duration_cast<chrono::nanoseconds>(end - start).count()) / 1e9;
}