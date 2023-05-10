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

int THREADS = 8;
int PER_THREAD = 2000;

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

    std::cout << "Generated Graph. n = " << g->n << ", m = " << g->m / 2 << endl;
}

void printGraph(graph *g)
{
    for (int i = 0; i < g->n; i++)
    {
        cout << g->vertices[i]->id << ": ";
        for (int j = 0; j < g->vertices[i]->edges.size(); j++)
        {
            cout << g->vertices[i]->edges[j]->v1->id << " " << g->vertices[i]->edges[j]->v2->id << " | ";
        }
        cout << endl;
    }
}

void runAlgorithms(graph *g, double &tv, double &tvp, double &tvuf, double &tvpuf, int iterations, bool edgeFlag = false)
{
    if (edgeFlag)
    {
        // run parallel algos to find number of edges in aux graph
        for (int i = 0; i < iterations; i++)
        {
            tv += int(tarjan_vishkin(g, edgeFlag));
            tvuf += int(tarjan_vishkin_uf(g, edgeFlag));
            // tvp += int(tarjan_vishkin_parallel(g, edgeFlag));
            // tvpuf += int(tarjan_vishkin_parallel_uf(g, edgeFlag));
        }

        return;
    }

    for (int i = 0; i < iterations; i++)
    {
        tv += tarjan_vishkin(g);

        tvp += tarjan_vishkin_parallel(g);

        tvuf += tarjan_vishkin_uf(g);

        tvpuf += tarjan_vishkin_parallel_uf(g);

        cout << "Iteration " << i << " done." << endl;
    }
}

void degree()
{
    ofstream myfile;
    myfile.open("results.csv", ios::trunc);
    myfile << "Vertices, Edges, Average Degree, Tarjan Vishkin, Tarjan Vishkin with Union Find, Tarjan Vishkin Parallel, Tarjan Vishkin Parallel with Union Find" << endl;

    int n;
    cout << "Enter number of vertices: ";
    cin >> n;

    vector<float> degrees_to_test = {0.1, 0.2, 0.5, 0.75, 1, 2, 4, 16, 64, 96, 128, 192, 256};

    for (int graphN = 0; graphN < degrees_to_test.size(); graphN++)
    {
        int m = n * degrees_to_test[graphN] / 2;
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

        myfile << n << "," << m << "," << g->avgDegree << "," << tv / iterations << "," << tvuf / iterations << "," << tvp / iterations << "," << tvpuf / iterations << endl;

        destroyGraph(g);
    }
}

void aux_edges()
{
    ofstream myfile;
    myfile.open("results.csv", ios::trunc);
    myfile << "Vertices, Edges, Average Degree, Tarjan Vishkin, Tarjan Vishkin with Union Find" << endl;

    int n;
    cout << "Enter number of vertices: ";
    cin >> n;

    vector<float> degrees_to_test = {0.25,0.5,1,2,2.25,2.5,2.75,3,4,5,6,7,8,9,10,12,14,16,20,24,32,48,64,96,128,142,196};

    for (int graphN = 0; graphN < degrees_to_test.size(); graphN++)
    {
        int m = n * degrees_to_test[graphN] / 2;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, n - 1);
        vector<int> degrees(n, 0);

        graph *g = new graph();
        g->n = n;
        g->m = m;

        generateRandomGraph(g, gen, dis, degrees);

        // print the graph
        // printGraph(g);

        double tv = 0, tvp = 0, tvuf = 0, tvpuf = 0;
        int iterations = 1;

        runAlgorithms(g, tv, tvp, tvuf, tvpuf, iterations, true);

        myfile << n << "," << m << "," << g->avgDegree << "," << tv << "," << tvuf << endl;

        destroyGraph(g);
    }
}

void aux_edges_file(char* path)
{
    ifstream input(path);
    cout << "Reading file: " << path << endl;

    ofstream myfile;
    myfile.open("results.csv", ios::app);

    /* Read number of vertices and edges */
    int n, m;
    input >> n >> m;

    /* Create graph */
    graph *g = new graph();
    g->n = n;
    g->m = m;

    loadGraph(g, input);
    input.close();

    double tv = 0, tvp = 0, tvuf = 0, tvpuf = 0;
    int iterations = 1;

    runAlgorithms(g, tv, tvp, tvuf, tvpuf, iterations, true);

    myfile << n << "," << m << "," << g->avgDegree << "," << tv << "," << tvuf << endl;

    destroyGraph(g);
}

void threads()
{
    ofstream myfile;
    myfile.open("results.csv", ios::trunc);
    myfile << "Vertices, Edges, Average Degree, Number of Threads, Tarjan Vishkin, Tarjan Vishkin with Union Find, Tarjan Vishkin Parallel, Tarjan Vishkin Parallel with Union Find" << endl;

    int n, m;
    cout << "enter number of vertices" << endl;
    cin >> n;
    cout << "enter number of edges" << endl;
    cin >> m;

    int numThreads[9] = {1, 2, 4, 8, 12, 16, 24, 32, 48};

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, n - 1);
    vector<int> degrees(n, 0);

    graph *g = new graph();
    g->n = n;
    g->m = m;

    generateRandomGraph(g, gen, dis, degrees);

    for (int threads : numThreads)
    {
        cout << "Running with " << threads << " threads." << endl;
        THREADS = threads;
        double tv = 0, tvp = 0, tvuf = 0, tvpuf = 0;
        int iterations = 20;

        runAlgorithms(g, tv, tvp, tvuf, tvpuf, iterations);

        myfile << n << "," << m << "," << g->avgDegree << "," << threads << "," << tv / iterations << "," << tvuf / iterations << "," << tvp / iterations << "," << tvpuf / iterations << endl;
    }

    destroyGraph(g);
}

void folderRead(char *path)
{
    ofstream myfile;
    myfile.open("results.csv", ios::trunc);
    myfile << "Vertices, Edges, Average Degree, File Name, Tarjan Vishkin, Tarjan Vishkin with Union Find, Tarjan Vishkin Parallel, Tarjan Vishkin Parallel with Union Find, Graph Name" << endl;

    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(path)) == NULL)
    {
        /* Could not open directory */
        std::cerr << "Could not open directory: " << path << std::endl;
        return;
    }

    /* Iterate over all files and directories within the folder */

    while ((ent = readdir(dir)) != NULL)
    {
        string file_path = get_file_path(path, ent->d_name);
        if (file_path == "")
            continue;
        cout << "Reading file: " << file_path << endl;
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

        runAlgorithms(g, tv, tvp, tvuf, tvpuf, iterations);
        myfile << n << "," << m << "," << g->avgDegree << "," << file_path << "," << tv / iterations << "," << tvuf / iterations << "," << tvp / iterations << "," << tvpuf / iterations << endl;
        destroyGraph(g);
    }
    closedir(dir);
}

void dense_graphs()
{
    ofstream myfile;
    myfile.open("results.csv", ios::trunc);
    myfile << "Vertices, Edges, Average Degree, Tarjan Vishkin, Tarjan Vishkin with Union Find, Tarjan Vishkin Parallel, Tarjan Vishkin Parallel with Union Find" << endl;

    int n, m;
    cout << "enter number of vertices to start from" << endl;
    cin >> n;

    for (int i = 0; i < 5; i++)
    {
        m = n * (n - 1) / 2;

        graph *g = new graph();
        g->n = n;
        g->m = m;

        // add n vertexes to g
        for (int i = 0; i < n; i++)
        {
            vertex *v = new vertex();
            v->id = i;
            g->vertices.push_back(v);
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                edge *e = new edge();
                e->v1 = g->vertices[i];
                e->v2 = g->vertices[j];
                g->edges.push_back(e);
                g->vertices[i]->edges.push_back(e);
                g->vertices[j]->edges.push_back(e);
            }
        }

        double tv = 0, tvp = 0, tvuf = 0, tvpuf = 0;
        int iterations = 10;

        runAlgorithms(g, tv, tvp, tvuf, tvpuf, iterations);

        myfile << n << "," << m << "," << g->avgDegree << "," << tv / iterations << "," << tvuf / iterations << "," << tvp / iterations << "," << tvpuf / iterations << endl;

        destroyGraph(g);

        n += 100;
    }
}

void fileRead(char *path)
{
    ifstream input(path);
    cout << "Reading file: " << path << endl;

    ofstream myfile;
    myfile.open("results.csv", ios::app);

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

    runAlgorithms(g, tv, tvp, tvuf, tvpuf, iterations);

    myfile << n << "," << m << "," << g->avgDegree << "," << path << "," << tv / iterations << "," << tvuf / iterations << "," << tvp / iterations << "," << tvpuf / iterations << endl;

    myfile.close();

    destroyGraph(g);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: ./main <path-to-folder/file>" << endl;
        return 0;
    }

    /* if argv is 1, we generate a random graph */

    if (strcmp(argv[1], "random") == 0)
    {
        if (argc > 3)
            THREADS = atoi(argv[3]);
        if (argc < 3)
        {
            std::cout << "Usage: ./main random <random-operation>" << endl;
            return 0;
        }
        switch (atoi(argv[2]))
        {
        case 1:
            degree();
            break;
        case 2:
            threads();
            break;
        case 3:
            dense_graphs();
            break;
        case 4:
            aux_edges();
            break;
        default:
            std::cout << "Usage: ./main random <random-operation>" << endl;
            return 0;
        }
        return 0;
    }

    else if (strcmp(argv[1], "file") == 0)
    {
        if (argc > 3)
            THREADS = atoi(argv[3]);
        if (argc < 3)
        {
            std::cout << "Usage: ./main file <path-to-file>" << endl;
            return 0;
        }

        fileRead(argv[2]);
    }else if (strcmp(argv[1],"file_aux") == 0){
        if (argc > 3)
            THREADS = atoi(argv[3]);
        if (argc < 3)
        {
            std::cout << "Usage: ./main file_aux <path-to-file>" << endl;
            return 0;
        }

        aux_edges_file(argv[2]);
    }
    else
    {
        if (argc > 2)
            THREADS = atoi(argv[2]);

        folderRead(argv[1]);
    }
    exit(0);
}