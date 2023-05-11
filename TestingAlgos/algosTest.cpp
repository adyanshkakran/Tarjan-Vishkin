#include "algosTest.h"

// Loads a Graph_test from a file
// The first line of the file is the number of vertices
// Remaining lines are the edges [fromVertex toVertex]
void loadGraph(const char *filename, Graph_test &g)
{
	ifstream fin;
	fin.open(filename);
	
	size_t V, E, u, v;
	fin >> V >> E;
	g.resize(V);
	
	while(!fin.fail())
	{
		fin >> u >> v;
		if(!fin.fail())
			g.addEdgeSafe(u, v);
	}
	
	fin.close();
}

// Connects a (potentially) unconnected Graph_test
void connectGraph(Graph_test &g)
{
	vector< vector<size_t> > components;
	g.spanningTree(NULL, NULL,  NULL, NULL, &components);
	for(size_t i = 1; i < components.size(); i++)
		g.addEdge(components[i-1][0], components[i][0]);
}

vector<double> testAlgos(char *path)
{
	size_t nthreads = 8;
	
	Graph_test g;
	loadGraph(path, g);
	connectGraph(g);
	
	double avgDegree = double(g.E() / 2) / g.V();
	size_t minDegree = INT_MAX;
	size_t maxDegree = 0;
	for(size_t i = 0; i < g.V(); i++)
	{
		if(g.adj(i).size() < minDegree)
			minDegree = g.adj(i).size();
		if(g.adj(i).size() > maxDegree)
			maxDegree = g.adj(i).size();
	}
	
	// cout << "Run stats ====================================\n"
	//      << "File:           " << argv[1] << "\n"
	//      << "Minimum degree: " << minDegree << "\n"
	//      << "Maximum degree: " << maxDegree << "\n"
	//      << "Average degree: " << avgDegree << "\n"
	//      << "Thread count:   " << nthreads << endl;
	
#ifdef DEBUG
	cout << "Graph_test:\n" << g << endl;
#endif
	
	vector<BiCC *> algorithms;
	algorithms.push_back(new ChaitanyaKothapalli(nthreads));
	algorithms.push_back(new CKTV(nthreads));

	vector<double> times;
	
	for(size_t i = 0; i < algorithms.size(); i++)
	{
		// cout << algorithms[i]->name() << endl;
		
		vector< set<size_t> > bicc;
		
		chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();
		algorithms[i]->getBiCC(g, bicc);
		chrono::high_resolution_clock::time_point stop_time = chrono::high_resolution_clock::now();
		size_t nanoseconds = chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
		
		// cout << "Took " << nanoseconds / 1e9 << " seconds" << endl;
		
		times.push_back(nanoseconds / 1e9);

		delete algorithms[i];
	}
	
	return times;
}
