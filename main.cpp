#include <iostream>
#include <fstream>
#include <dirent.h>
#include "utils.h"
#include "tarjan-vishkin.h"
#include "tarjan-vishkin-uf.h"
#include "tarjan.h"
#include "tarjan-vishkin-parallel.h"
#include "tv-parallel-union-find.h"

using namespace std;

string get_file_path(const char *path, const char *file_name)
{
    /* Only open .in files */
    if (strstr(file_name, ".in") == NULL)
    {
        return "";
    }

    /* Create file path */
    string file_path = path;
    file_path += "/";
    file_path += file_name;

    return file_path;
}

void addEdgesFromFile(graph *g, ifstream &input, vector<int> &degrees)
{
    int u, v;
    for (int i = 0; i < g->m; i++)
    {
        input >> u >> v;
        edge *e = new edge();
        e->v1 = g->vertices[u];
        e->v2 = g->vertices[v];

        /* update degrees */
        degrees[u]++;
        degrees[v]++;

        g->vertices[u]->edges.push_back(e);
        g->vertices[v]->edges.push_back(e);
        g->edges.push_back(e);
        g->edges.push_back(reverseEdge(e));
    }
}

void loadGraph(graph *g, ifstream &input)
{
    /* Create vertices */
    for (int i = 0; i < g->n; i++)
    {
        vertex *v = new vertex();
        v->id = i;
        g->vertices.push_back(v);
    }

    vector<int> degrees(g->n, 0);

    addEdgesFromFile(g, input, degrees);

    g->m *= 2; // undirected graph

    /* Calculate average degree */
    g->avgDegree = 0;
    for (int i = 0; i < g->n; i++)
    {
        g->avgDegree += degrees[i];
    }
    g->avgDegree /= g->n;
}

void generateRandomGraph(graph *g, mt19937 &gen, uniform_int_distribution<> &dis, vector<int> &degrees)
{
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
        while (!corr)
        {
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
                    while (u == v)
                        v = dis(gen);
                    corr = false;
                    break;
                }
            }
        }
        e->v1 = g->vertices[u];
        e->v2 = g->vertices[v];
        degrees[u]++;
        degrees[v]++;
        g->vertices[u]->edges.push_back(e);
        g->vertices[v]->edges.push_back(e);
        g->edges.push_back(e);
        g->edges.push_back(reverseEdge(e));
    }
    g->m *= 2;
    g->avgDegree = 0;
    for (int i = 0; i < g->n; i++)
    {
        g->avgDegree += degrees[i];
    }
    g->avgDegree /= g->n;

    std::cout << "Generated Graph. n = " << g->n << ", m = " << g->m/2 << endl;
}

void printGraph(graph *g)
{
    for (int i = 0; i < 5; i++)
    {
        cout << g->vertices[i]->id << ": ";
        for (int j = 0; j < g->vertices[i]->edges.size(); j++)
        {
            cout << g->vertices[i]->edges[j]->v1->id << " " << g->vertices[i]->edges[j]->v2->id << " | ";
        }
        cout << endl;
    }
}

void runAlgorithms(graph *g, double &tv, double &tvp, double &tvuf, double &tvpuf, int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        tv += tarjan_vishkin(g);

        tvp += tarjan_vishkin_parallel(g);

        tvuf += tarjan_vishkin_uf(g);

        tvpuf += tarjan_vishkin_parallel_uf(g);

        cout << "Iteration " << i << " done." << endl;
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: ./main <path-to-folder>" << endl;
        return 0;
    }

    ofstream myfile;
    // open in write mode
    myfile.open("results.csv", ios::trunc);
    myfile << "Vertices, Edges, Average Degree, Tarjan Vishkin, Tarjan Vishkin with Union Find, Tarjan Vishkin Parallel, Tarjan Vishkin Parallel with Union Find, Graph Name" << endl;

    /* if argv is 1, we generate a random graph */

    if (strcmp(argv[1], "random") == 0)
    {
        int n;
        cout << "enter number of vertices" << endl;
        cin >> n;

        for (int graphN = 0; graphN < 10; graphN++)
        {
            int m = graphN * 10000 + n;

            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, n - 1);
            vector<int> degrees(n, 0);

            graph *g = new graph();
            g->n = n;
            g->m = m;

            generateRandomGraph(g, gen, dis, degrees);

            double tv = 0, tvp = 0, tvuf = 0, tvpuf = 0;
            int iterations = 20;

            runAlgorithms(g, tv, tvp, tvuf, tvpuf, iterations);

            myfile << n << "," << m << "," << g->avgDegree << "," << tv / 50 << "," << tvuf / 50 << "," << tvp / 50 << "," << tvpuf / 50 << endl;
        }

        return 0;
    }

    DIR *dir;
    struct dirent *ent;
    const char *path = argv[1];

    if ((dir = opendir(path)) == NULL)
    {
        /* Could not open directory */
        std::cerr << "Could not open directory: " << path << std::endl;
        return EXIT_FAILURE;
    }

    /* Iterate over all files and directories within the folder */

    while ((ent = readdir(dir)) != NULL)
    {
        string file_path = get_file_path(path, ent->d_name);
        if (file_path == "")
            continue;
        ifstream input(file_path);

        /* Read number of vertices and edges */
        int n, m;
        input >> n >> m;

        /* Create graph */
        graph *g = new graph();
        g->n = n;
        g->m = m;

        loadGraph(g, input);
        /* Close file */
        input.close();

        double tv = 0, tvp = 0, tvuf = 0, tvpuf = 0;
        int iterations = 20;

        printGraph(g);

        runAlgorithms(g, tv, tvp, tvuf, tvpuf, iterations);
        myfile << n << "," << m << "," << g->avgDegree << "," << tv / 50 << "," << tvuf / 50 << "," << tvp / 50 << "," << tvpuf / 50 << endl;
        destroyGraph(g);
    }
    closedir(dir);

    exit(0);
}