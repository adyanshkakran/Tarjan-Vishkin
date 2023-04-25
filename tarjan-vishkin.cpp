#include <bits/stdc++.h>
#include <omp.h>

using namespace std;
typedef long long ll;

typedef struct Edge edge;

typedef struct Vertex{
    ll id;
    vector<edge*> edges;
} vertex;

typedef struct Edge{
    vertex *v1, *v2;
} edge;

typedef struct Graph{
    ll n, m;
    vector<vertex*> vertices;
    vector<edge*> edges;
} graph;

edge* reverseEdge(edge* e){
    edge* rev = new edge();
    rev->v1 = e->v2;
    rev->v2 = e->v1;
    return rev;
}

bool cmp(edge* a, edge* b) {
    if(a->v1->id == b->v1->id)
        return a->v2->id < b->v2->id;
    return a->v1->id < b->v1->id;
}

bool cmp2(edge* a, edge* b) {
    return a->v1->id == b->v1->id && a->v2->id == b->v2->id;
}

ll LCA(vector<ll>& level, vector<ll>& parent, ll u, ll v) {
    if(level[u] < level[v])
        swap(u, v);
    while(level[u] > level[v])
        u = parent[u];
    while(u != v) {
        u = parent[u];
        v = parent[v];
    }
    return u;
}

void bfs(graph* g, graph* t, graph* nt, vector<ll>& parent, vector<ll>& level) {
    queue<vertex*> q;
    vector<bool> discovered(g->n, false), visited(g->n, false);

    q.push(g->vertices[0]);
    discovered[0] = true;

    for(vertex* i : g->vertices) {
        vertex* tt = new vertex();
        tt->id = i->id;
        t->vertices.push_back(tt);
    }

    for(vertex* i : g->vertices) {
        vertex* tt = new vertex();
        tt->id = i->id;
        nt->vertices.push_back(tt);
    }

    for(int i = 0; i < g->n; i++){
        if(!visited[i]){
            q.push(g->vertices[i]);

            parent[g->vertices[i]->id] = g->vertices[i]->id;
            level[g->vertices[i]->id] = 0;
                
            discovered[g->vertices[i]->id] = true;
            while (!q.empty()) {
                vertex* v = q.front();
                q.pop();

                if(visited[v->id])
                    continue;
                visited[v->id] = true;

                for (edge* e : v->edges) {
                    vertex* u = e->v1 == v ? e->v2 : e->v1;
                    if (!visited[u->id]) {
                        if (!discovered[u->id]) {
                            discovered[u->id] = true;
                            q.push(u);

                            level[u->id] = level[v->id] + 1;
                            parent[u->id] = v->id;

                            edge* rev = reverseEdge(e);
                            t->edges.push_back(e);
                            t->edges.push_back(rev);
                            t->vertices[u->id]->edges.push_back(e);
                            t->vertices[v->id]->edges.push_back(rev);
                        }else{
                            edge* rev = reverseEdge(e);
                            nt->edges.push_back(e);
                            nt->edges.push_back(rev);
                            nt->vertices[u->id]->edges.push_back(e);
                            nt->vertices[v->id]->edges.push_back(rev);
                        }
                    }
                }
            }
        }
    }
}

void findConnectedComponents(graph* g, vector<vector<ll>>& component) {
    queue<vertex*> q;
    vector<bool> discovered(g->n, false), visited(g->n, false);

    q.push(g->vertices[0]);
    discovered[0] = true;

    for(int i = 0; i < g->n; i++){
        if(!visited[i]){
            q.push(g->vertices[i]);
            discovered[g->vertices[i]->id] = true;

            component.push_back(vector<ll>(1, g->vertices[i]->id));
            while (!q.empty()) {
                vertex* v = q.front();
                q.pop();

                if(visited[v->id])
                    continue;
                visited[v->id] = true;

                for (edge* e : v->edges) {
                    vertex* u = e->v1 == v ? e->v2 : e->v1;
                    if (!visited[u->id]) {
                        if (!discovered[u->id]) {
                            discovered[u->id] = true;
                            q.push(u);
                            component.back().push_back(u->id);
                        }
                    }
                }
            }
        }
    }
}

void euler_tour(graph* t, vector<ll>& succ) {
    vector<ll> first(t->n, -1), next(t->m, -1), twin(t->m, -1);
    for(ll i = 0; i < t->m; i++) {
        twin[distance(t->edges.begin(), lower_bound(t->edges.begin(), t->edges.end(), reverseEdge(t->edges[i]), cmp))] = i;

        if(i == 0 || t->edges[i]->v1->id != t->edges[i-1]->v1->id)
			first[t->edges[i]->v1->id] = i;
		else if(t->edges[i]->v1->id == t->edges[i-1]->v1->id)
			next[i-1] = i;
    }
    succ.resize(t->m, -1);
    for(ll i = 0; i < t->m; i++) {
        if(next[twin[i]] != -1)
            succ[i] = next[twin[i]];
        else
            succ[i] = first[t->edges[i]->v2->id];
    }
}

void preOrderVertices(graph* t, vector<ll>& pre, vector<ll>& succ) {
    pre.resize(t->n, -1);
    pre[t->edges[0]->v1->id] = 0;
    pre[t->edges[0]->v2->id] = 1;

    ll i = 0, j = 2;
    do
    {
        ll k = t->edges[succ[i]]->v2->id;
        if (pre[k] == -1)
            pre[k] = j++;
        i = succ[i];
    } while (i != 0);
}

void findLow(graph* t, graph* nt, vector<ll>& low, vector<ll>& level){
    low.resize(nt->n);

    for(ll i = 0; i < nt->n; i++)
        low[i] = i;

    bool changed = true;
    while(changed) {
        changed = false;

        for(ll i = 0; i < nt->n; i++) {
            for (edge* e : t->vertices[i]->edges) {

                ll j = e->v1->id == i ? e->v2->id : e->v1->id;

                if (level[i] < level[j] && level[low[j]] < level[low[i]]) {
                    low[i] = low[j];
                    changed = true;
                }
            }

            for(edge* e : nt->vertices[i]->edges) {

                ll j = e->v1->id == i ? e->v2->id : e->v1->id;

                if (level[low[j]] < level[low[i]]) {
                    low[i] = low[j];
                    changed = true;
                }
            }
        }
    }
}

graph* auxillaryGraph(graph* g, graph* t, graph *nt,vector<ll>& low, vector<ll>& level, vector<ll>& parent, vector<ll>& pre) {
    graph* aux = new graph();
    aux->n = g->n + nt->m;
    for(ll i = 0; i < aux->n; i++) {
        vertex* v = new vertex();
        v->id = i;
        aux->vertices.push_back(v);
    }
    vector<ll> num(g->m,0), pref(g->m,0);
    ll nti = 0, ti = 0;
    for(ll i = 0; i < g->m; i++){
		if(nti >= nt->m || (ti < t->m && g->edges[i]->v1->id == t->edges[ti]->v1->id && g->edges[i]->v2->id == t->edges[ti]->v2->id))
			ti++;
		else
		{
			num[i] = 1;
			nti++;
		}
	}

    pref[0] = num[0];
    for(ll i = 1; i < g->m; i++)
        pref[i] = pref[i-1] + num[i];
    
    ti = 0; nti = 0;
    for(ll i = 0; i < g->m; i++) {
        ll u = g->edges[i]->v1->id, v = g->edges[i]->v2->id;
        if( nti >= nt->edges.size() || (ti < t->m && g->edges[i]->v1 == t->edges[ti]->v1 && g->edges[i]->v2 == t->edges[ti]->v2)){
            if(level[u] < level[v] && level[low[v]] <= level[u]){
                edge* e = new edge();
                e->v1 = aux->vertices[u];
                e->v2 = aux->vertices[v];
                aux->edges.push_back(e);
                aux->vertices[u]->edges.push_back(e);
                aux->vertices[v]->edges.push_back(e);
            }
            ti++;   
        }else{
            ll p = LCA(level,parent, u, v);
            if(pre[v] < pre[u]) {
                edge* e = new edge();
                e->v1 = aux->vertices[u];
                e->v2 = aux->vertices[pref[i] + g->n -1];
                aux->edges.push_back(e);
                aux->vertices[u]->edges.push_back(e);
                aux->vertices[pref[i] + g->n -1]->edges.push_back(e);
            }
            if(u < v && p != u && p != v){
                edge* e = new edge();
                e->v1 = aux->vertices[u];
                e->v2 = aux->vertices[v];
                aux->edges.push_back(e);
                aux->vertices[u]->edges.push_back(e);
                aux->vertices[v]->edges.push_back(e);
            }
            nti++;
        }
    }
    return aux;
}

void remapAuxGraph(graph* t, vector<set<ll>>& bi, vector<vector<ll>>& components, vector<ll>& parent) {
    bi.resize(components.size() - 1);
	for(ll i = 1; i < components.size(); i++)
	{
		for(ll j = 0; j < components[i].size(); j++)
		{
			if(components[i][j] < t->n)
			{
				bi[i-1].insert(components[i][j]);
				bi[i-1].insert(parent[components[i][j]]);
			}
		}
	}
}

int main() {
    ifstream cin("datasets/graph1.txt");
    graph *g = new graph();
    cin >> g->n >> g->m;
    for (int i = 0; i < g->n; i++) {
        vertex *v = new vertex();
        v->id = i;
        g->vertices.push_back(v);
    }
    for (int i = 0; i < g->m; i++) {
        edge *e = new edge();
        ll u, v;
        cin >> u >> v;
        e->v1 = g->vertices[u];
        e->v2 = g->vertices[v];
        g->vertices[u]->edges.push_back(e);
        g->vertices[v]->edges.push_back(e);
        g->edges.push_back(e);
        g->edges.push_back(reverseEdge(e));
    }
    g->m *= 2;
    graph *t = new graph(), *nt = new graph();
    t->n = g->n;
    nt->n = g->n;
    vector<ll> pre, succ, low, parent(g->n, -1), level(g->n, -1);
    bfs(g,t,nt, parent, level);
    sort(g->edges.begin(), g->edges.end(), cmp);
    sort(t->edges.begin(), t->edges.end(), cmp);
    sort(nt->edges.begin(), nt->edges.end(), cmp);

 
    t->m = t->edges.size();
    nt->m = nt->edges.size();

    // for(ll i = 0; i < t->m; i++)
    //     cout << t->edges[i]->v1->id << " " << t->edges[i]->v2->id << endl;

    euler_tour(t, succ);

    preOrderVertices(t, pre, succ);
 
    findLow(t, nt, low, level);

    graph* aux = auxillaryGraph(g, t, nt, low, level, parent, pre);

    vector<vector<ll>> connected;
    findConnectedComponents(aux, connected);

    vector<set<ll>> biconnected;
    remapAuxGraph(t, biconnected, connected, parent);

    for(ll i = 0; i < biconnected.size(); i++) {
        if(biconnected[i].size() == 0){
            biconnected.erase(biconnected.begin() + i);
            i--;
        }
    }
    // cout << t->n << " " << t->m << endl;
    // cout << nt->n << " " << nt->m << endl;
    // cout << aux->n << " " << aux->m << endl;
    for (set<ll> s : biconnected) {
        for (ll i : s)
            cout << i << " ";
        cout << endl;
    }
    return 0;
}