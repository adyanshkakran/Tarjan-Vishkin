#include <iostream>
#include <fstream>
#include "utils.h"
#include "tarjan-vishkin.h"
#include "tarjan-vishkin-uf.h"
#include "tarjan.h"
#include "tarjan-vishkin-parallel.h"
#include "tv-parallel-union-find.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: ./main <n> <m>" << endl;
        return 0;
    }

    int n = stoi(argv[1]), m = stoi(argv[2]);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, n-1);


    graph *g = new graph();
    g->n = n;
    g->m = m;
    // cin >> g->n >> g->m;
    for (int i = 0; i < g->n; i++)
    {
        vertex *v = new vertex();
        v->id = i;
        g->vertices.push_back(v);
    }
    for (int i = 0; i < 2 * g->m; i += 2)
    {
        edge *e = new edge();
        ll u = dis(gen), v = dis(gen);
        bool corr = false;
        while(!corr){
            corr = true;
            while (u == v)
            {
                v = dis(gen);
            }
            for (int j = 0; j < g->vertices[u]->edges.size(); j++)
            {
                if (g->vertices[u]->edges[j]->v2->id == v || g->vertices[u]->edges[j]->v1->id == v)
                {
                    v = dis(gen);
                    while(u == v)
                        v = dis(gen);
                    corr = false;
                    break;
                }
            }
        }
        e->v1 = g->vertices[u];
        e->v2 = g->vertices[v];
        g->vertices[u]->edges.push_back(e);
        g->vertices[v]->edges.push_back(e);
        g->edges.push_back(e);
        g->edges.push_back(reverseEdge(e));
    }
    g->m *= 2;

    std::cout << "Generated Graph" << endl;
    // print graph
    // for (int i = 0; i < g->n; i++)
    // {
    //     std::cout << g->vertices[i]->id << ": ";
    //     for (int j = 0; j < g->vertices[i]->edges.size(); j++)
    //     {
    //         std::cout << (g->vertices[i]->edges[j]->v2->id == g->vertices[i]->id ? g->vertices[i]->edges[j]->v1->id : g->vertices[i]->edges[j]->v2->id) << " ";
    //     }
    //     std::cout << endl;
    // }
    double tv = 0, tvp = 0, tvuf = 0, tvpuf = 0;

    for(int i = 0; i < 50; i++){
        tv += tarjan_vishkin(g);

        tvp += tarjan_vishkin_parallel(g);

        tvuf += tarjan_vishkin_uf(g);

        tvpuf += tarjan_vishkin_parallel_uf(g);
        cout << "Iteration " << i << " done." << endl;
    }
    std::cout << "tarjan-vishkin: " << tv/50 << endl;
    std::cout << "tarjan-vishkin-parallel: " << tvp/50 << endl;
    std::cout << "tarjan-vishkin-uf: " << tvuf/50 << endl;
    std::cout << "tarjan-vishkin-parallel-uf: " << tvpuf/50 << endl;
}