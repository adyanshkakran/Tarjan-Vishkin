#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <chrono>
#include <omp.h>

using namespace std;

// Graph class to represent the graph
class Graph {
public:
    int V;
    vector<vector<int>> adj;

    // Constructor
    Graph()
    {
    }

    Graph(int V)
    {
        this->V = V;
        adj.resize(V);
    }

    // Function to add an edge to the graph
    void addEdge(int v, int w)
    {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    // Sequential BFS function
    void bfsSeq(int start)
    {
        cout << "Sequential BFS" << endl;
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }

    // Parallelized BFS function
    void bfsPar(int start)
    {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        #pragma omp parallel
        {
            while (!q.empty()) {
                cout << "h" << endl;
                #pragma omp for
                for (int i = 0; i < q.size(); i++) {
                    int u = q.front();
                    cout << u << endl;
                    q.pop();

                    for (auto v : adj[u]) {
                        if (!visited[v]) {
                            visited[v] = true;
                            q.push(v);
                        }
                    }
                }
            }
        }
    }

    // Function to print BFS traversal from a given source vertex
    void printBFS(int s)
    {
        cout << "BFS starting from vertex " << s << ":\n";

        // auto startSeq = chrono::high_resolution_clock::now();
        // bfsSeq(s);
        // auto endSeq = chrono::high_resolution_clock::now();

        auto startPar = chrono::high_resolution_clock::now();
        bfsPar(s);
        auto endPar = chrono::high_resolution_clock::now();

        // cout << "Sequential BFS took "
        //      << chrono::duration_cast<chrono::microseconds>(endSeq - startSeq).count()
        //      << " microseconds.\n";
        cout << "Parallel BFS took "
             << chrono::duration_cast<chrono::microseconds>(endPar - startPar).count()
             << " microseconds.\n";
    }
};

// Function to read the input file and construct the graph
void readGraph(string filename, Graph& g)
{
    ifstream file(filename);

    if (file) {
        int V, E;
        file >> V >> E;

        g = Graph(V);

        int u, v;
        for (int i = 0; i < E; i++) {
            file >> u >> v;
            g.addEdge(u, v);
        }
    }
    else {
        cout << "Error: Unable to open file " << filename << endl;
    }
}

// Main function
int main()
{
    string filename = "./datasets/graph1.txt";
    Graph g;
    readGraph(filename, g);
    g.printBFS(0);

    return 0;
}
