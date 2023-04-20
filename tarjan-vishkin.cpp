#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

typedef struct Edge edge;

typedef struct Vertex{
    ll id, level;
    struct Vertex* parent;
    vector<struct Vertex*> children;
    vector<edge*> edges;
} vertex;

typedef struct Edge{
    // ll id;
    vertex *v1, *v2;
} edge;

typedef struct Graph{
    ll n, m;
    vector<vertex*> vertices;
    vector<edge*> edges;
} graph;

edge* reverseEdge(edge* e){
    edge* rev = new edge();
    // rev->id = id;
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

void bfs(graph* g, graph* t, graph* nt) {
    queue<vertex*> q;
    vector<bool> discovered(g->n, false), visited(g->n, false);

    for(vertex* i : g->vertices) {
        vertex* tt = new vertex();
        tt->id = i->id;
        t->vertices.push_back(tt);
        vertex* ntt = new vertex();
        ntt->id = i->id;
        nt->vertices.push_back(ntt);
    }

    for(int i = 0; i < g->n; i++){
        if(!visited[i]){
            q.push(g->vertices[i]);
            g->vertices[i]->parent = g->vertices[i];
            g->vertices[i]->level = 0;
            discovered[g->vertices[i]->id] = true;
            while (!q.empty()) {
                vertex* v = q.front();

                if(visited[v->id])
                    continue;
                visited[v->id] = true;

                q.pop();
                for (edge* e : v->edges) {
                    vertex* u = e->v1 == v ? e->v2 : e->v1;
                    if (!visited[u->id]) {
                        if (!discovered[u->id]) {
                            discovered[u->id] = true;
                            q.push(u);

                            u->level = v->level + 1;
                            u->parent = v;
                            v->children.push_back(u);

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
    low.resize(t->n);
    for(ll i = 0; i < t->n; i++)
        low[i] = i;

    bool changed = true;
    while(changed) {
        changed = false;

        for(ll i = 0; i < t->n; i++) {
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

graph* auxillaryGraph(graph* g, graph* t, graph *nt) {
    graph* aux = new graph();
    aux->n = g->n + nt->m;
    for(ll i = 0; i < aux->n; i++) {
        vertex* v = new vertex();
        v->id = i;
        aux->vertices.push_back(v);
    }
    vector<ll> num(g->m,0);
    ll nti = 0, ti = 0;
    for(ll i = 0; i < g->m; i++){
		if(nti >= nt->m || (g->edges[i]->v1 == t->edges[ti]->v1 && g->edges[i]->v2 == t->edges[ti]->v2))
			ti++;
		else
		{
			num[i] = 1;
			nti++;
		}
	}
    return aux;
}

int main() {
    ifstream cin("datasets/graph1.txt");
    graph *g = new graph();
    cin >> g->n >> g->m;
    for (int i = 0; i < g->n; i++) {
        vertex *v = new vertex();
        v->id = i;
        v->parent = NULL;
        g->vertices.push_back(v);
    }
    for (int i = 0; i < g->m; i++) {
        edge *e = new edge();
        // e->id = i*2;
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
    t->m = 2*(g->n-1);
    nt->m = g->m - t->m;
    bfs(g,t,nt);
    vector<ll> pre, succ, low;
    sort(g->edges.begin(), g->edges.end(), cmp);
    sort(t->edges.begin(), t->edges.end(), cmp);
    sort(nt->edges.begin(), nt->edges.end(), cmp);
    // for (edge* e : g->edges)
    //     cout << e->v1->id << " " << e->v2->id << endl;
    // cout << endl;
    // for (edge* e : t->edges)
    //     cout << e->v1->id << " " << e->v2->id << endl;
    // cout << endl;
    // for (edge* e : nt->edges)
    //     cout << e->v1->id << " " << e->v2->id << endl;
    euler_tour(t, succ);
    // for (ll i : succ)
    //     cout << i << " ";
    // cout << endl;
    preOrderVertices(t, pre, succ);
    // for (ll i : pre)
    //     cout << i << " ";
    // cout << endl;
    findLow(t, nt, low, pre);
    // for (ll i : low)
    //     cout << i << " ";
    // cout << endl;
    
    return 0;
}