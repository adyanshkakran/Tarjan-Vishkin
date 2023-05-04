#include <iostream>
#include <fstream>
#include <vector>
#include <random>

using namespace std;

int main() {
    int n = 10000; // number of vertices
    int m = 20000; // number of edges
    // cin >> n >> m;

    ofstream cout;
    cout.open("datasets/graph1.txt");

    vector<vector<int>> graph = generateGraph(n, m);

    // Print the graph
    cout << n << " " << m << endl;
    for (int u = 0; u < n; u++) {
        for (int v : graph[u]) {
            cout << u << " " << v << endl;
        }
    }

    return 0;
}
