#include "graph.h"
#include <queue>
#include <algorithm>
using namespace std;

void Graph_test::resize(size_t v)
{
	m_adj.resize(v);
}

void Graph_test::addVertex()
{
	m_adj.push_back(list<size_t>());
}

void Graph_test::addEdge(size_t u, size_t v)
{
	addDirectedEdge(u, v);
	addDirectedEdge(v, u);
}

// adds Edge_test (u, v), keeping m_edges sorted lexicographically
void Graph_test::addDirectedEdge(size_t u, size_t v)
{
	Edge_test e(u, v);
	m_edges.insert(lower_bound(m_edges.begin(), m_edges.end(), e), e);
	m_adj[u].push_back(v);
}

void Graph_test::addEdgeSafe(size_t u, size_t v)
{
	addDirectedEdgeSafe(u, v);
	addDirectedEdgeSafe(v, u);
}

void Graph_test::addDirectedEdgeSafe(size_t u, size_t v)
{
	Edge_test e(u, v);
	vector<Edge_test>::iterator i = lower_bound(m_edges.begin(), m_edges.end(), e);
	if(i == m_edges.end() || *i != e)
	{
		m_edges.insert(i, e);
		m_adj[u].push_back(v);
	}
}

void Graph_test::removeEdge(size_t u, size_t v)
{
	removeDirectedEdge(u, v);
	removeDirectedEdge(v, u);
}

void Graph_test::removeDirectedEdge(size_t u, size_t v)
{
	Edge_test e(u, v);
	m_edges.erase(lower_bound(m_edges.begin(), m_edges.end(), e));
	m_adj[u].erase(find(m_adj[u].begin(), m_adj[u].end(), v));
}

void Graph_test::removeEdgeSafe(size_t u, size_t v)
{
	removeDirectedEdgeSafe(u, v);
	removeDirectedEdgeSafe(v, u);
}

void Graph_test::removeDirectedEdgeSafe(size_t u, size_t v)
{
	Edge_test e(u, v);
	vector<Edge_test>::iterator i = find(m_edges.begin(), m_edges.end(), e);
	if(i != m_edges.end())
	{
		m_edges.erase(i);
		m_adj[u].erase(find(m_adj[u].begin(), m_adj[u].end(), v));
	}
}

list<size_t> &Graph_test::adj(size_t u)
{
	return m_adj[u];
}

const list<size_t> &Graph_test::adj(size_t u) const
{
	return m_adj[u];
}

const vector<Edge_test> &Graph_test::edges() const
{
	return m_edges;
}

size_t Graph_test::V() const
{
	return m_adj.size();
}

size_t Graph_test::E() const
{
	return m_edges.size();
}

void Graph_test::copyComponent(const Graph_test &g, const vector<size_t> &component, vector<size_t> &antiAlias, vector<size_t> &alias)
{
	resize(component.size());
	antiAlias.resize(g.V(), -1);
	alias.resize(component.size());
	for(size_t i = 0; i < component.size(); i++)
	{
		antiAlias[component[i]] = i;
		alias[i] = component[i];
	}
	for(size_t u = 0; u < component.size(); u++)
	{
		for(list<size_t>::const_iterator j = g.adj(component[u]).begin(); j != g.adj(component[u]).end(); ++j)
		{
			size_t v = antiAlias[*j];
			if(v != -1)
				addDirectedEdge(u, v);
		}
	}
}

void Graph_test::spanningTree(Graph_test *t, Graph_test *nt, vector<size_t> *parent, vector<size_t> *level, vector< vector<size_t> > *components) const
{
	vector<bool> visited(V(), false), discovered(V(), false);
	
	queue<size_t> q;
	q.push(0);
	discovered[0] = true;
	
	if(t)
		t->resize(V());
	if(nt)
		nt->resize(V());
	
	for(size_t i = 0; i < V(); i++)
	{
		if(!visited[i])
		{
			q.push(i);
			discovered[i] = true;
			
			if(parent)
			{
				parent->resize(V());
				(*parent)[i] = i;
			}
			if(level)
			{
				level->resize(V());
				(*level)[i] = i;
			}
			if(components)
				components->push_back(vector<size_t>(1, i));
			
			while(!q.empty())
			{
				size_t u = q.front();
				q.pop();
				
				if(visited[u])
					continue;
				visited[u] = true;
				
				for(list<size_t>::const_iterator j = adj(u).begin(); j != adj(u).end(); ++j)
				{
					size_t v = *j;
					if(!visited[v])
					{
						if(!discovered[v])
						{
							discovered[v] = true;
							q.push(v);
							
							if(t)
								t->addEdge(u, v);
							if(parent)
								(*parent)[v] = u;
							if(level)
								(*level)[v] = (*level)[u] + 1;
							if(components)
								components->back().push_back(*j);
						}
						else if(nt)
							nt->addEdge(u, v);
					}
				}
			}
		}
	}
}
