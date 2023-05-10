#include "utils.h"

int PER_THREAD;
int THREADS;

ll ceil(ll a, ll b)
{
    if (b == 0)
        return -1;
    return a % b == 0 ? a / b : a / b + 1;
}

void destroyGraph(Graph *g)
{
    for (int i = 0; i < g->n; i++)
    {
        delete g->vertices[i];
    }
    g->vertices.clear();
    for (int i = 0; i < g->m; i++)
    {
        delete g->edges[i];
    }
    g->edges.clear();
    delete g;
}

edge *reverseEdge(edge *e)
{
    edge *rev = new edge();
    rev->v1 = e->v2;
    rev->v2 = e->v1;
    return rev;
}

bool cmp(edge *a, edge *b)
{
    if (a->v1->id == b->v1->id)
        return a->v2->id < b->v2->id;
    return a->v1->id < b->v1->id;
}

bool cmp2(edge *a, edge *b)
{
    return a->v1->id == b->v1->id && a->v2->id == b->v2->id;
}

ll LCA(vector<ll> &level, vector<ll> &parent, ll u, ll v)
{
    if (level[u] < level[v])
        swap(u, v);
    while (level[u] > level[v])
        u = parent[u];
    while (u != v)
    {
        u = parent[u];
        v = parent[v];
    }
    return u;
}

void bfs(graph *g, graph *t, graph *nt, vector<ll> &parent, vector<ll> &level)
{
    queue<vertex *> q;
    vector<bool> discovered(g->n, false), visited(g->n, false);

    q.push(g->vertices[0]);
    discovered[0] = true;

    for (vertex *i : g->vertices)
    {
        vertex *tt = new vertex();
        tt->id = i->id;
        t->vertices.push_back(tt);
    }

    for (vertex *i : g->vertices)
    {
        vertex *tt = new vertex();
        tt->id = i->id;
        nt->vertices.push_back(tt);
    }

    for (int i = 0; i < g->n; i++)
    {
        if (!visited[i])
        {
            q.push(g->vertices[i]);

            parent[g->vertices[i]->id] = g->vertices[i]->id;
            level[g->vertices[i]->id] = 0;

            discovered[g->vertices[i]->id] = true;
            while (!q.empty())
            {
                vertex *v = q.front();
                q.pop();

                if (visited[v->id])
                    continue;
                visited[v->id] = true;

                for (edge *e : v->edges)
                {
                    vertex *u = e->v1 == v ? e->v2 : e->v1;
                    if (!visited[u->id])
                    {
                        if (!discovered[u->id])
                        {
                            discovered[u->id] = true;
                            q.push(u);

                            level[u->id] = level[v->id] + 1;
                            parent[u->id] = v->id;

                            edge *ee = new edge();
                            ee->v1 = e->v1;
                            ee->v2 = e->v2;
                            edge *rev = reverseEdge(ee);
                            t->edges.push_back(ee);
                            t->edges.push_back(rev);
                            t->vertices[u->id]->edges.push_back(ee);
                            t->vertices[v->id]->edges.push_back(rev);
                        }
                        else
                        {
                            edge *ee = new edge();
                            ee->v1 = e->v1;
                            ee->v2 = e->v2;
                            edge *rev = reverseEdge(ee);
                            nt->edges.push_back(ee);
                            nt->edges.push_back(rev);
                            nt->vertices[u->id]->edges.push_back(ee);
                            nt->vertices[v->id]->edges.push_back(rev);
                        }
                    }
                }
            }
        }
    }
}

void findConnectedComponents(graph *g, vector<vector<ll>> &component)
{
    queue<vertex *> q;
    vector<bool> discovered(g->n, false), visited(g->n, false);

    q.push(g->vertices[0]);
    discovered[0] = true;

    for (int i = 0; i < g->n; i++)
    {
        if (!visited[i])
        {
            q.push(g->vertices[i]);
            discovered[g->vertices[i]->id] = true;

            component.push_back(vector<ll>(1, g->vertices[i]->id));
            while (!q.empty())
            {
                vertex *v = q.front();
                q.pop();

                if (visited[v->id])
                    continue;
                visited[v->id] = true;

                for (edge *e : v->edges)
                {
                    vertex *u = e->v1 == v ? e->v2 : e->v1;
                    if (!visited[u->id])
                    {
                        if (!discovered[u->id])
                        {
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

bool findEdgeById(const vector<edge *> &edges, edge *e)
{
    auto it = lower_bound(edges.begin(), edges.end(), e, cmp);

    if (it != edges.end() && (*it)->v1->id == e->v1->id && (*it)->v2->id == e->v2->id)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void euler_tour(graph *t, vector<ll> &succ)
{
    vector<ll> first(t->n, -1), next(t->m, -1), twin(t->m, -1);
    for (ll i = 0; i < t->m; i++)
    {
        vertex *temp = t->edges[i]->v1;
        edge *rev = reverseEdge(t->edges[i]);
        twin[distance(t->edges.begin(), lower_bound(t->edges.begin(), t->edges.end(), rev, cmp))] = i;
        delete (rev);

        if (i == 0 || t->edges[i]->v1->id != t->edges[i - 1]->v1->id)
            first[t->edges[i]->v1->id] = i;
        else if (t->edges[i]->v1->id == t->edges[i - 1]->v1->id)
            next[i - 1] = i;
    }
    succ.resize(t->m, -1);
    for (ll i = 0; i < t->m; i++)
    {
        if (next[twin[i]] != -1)
            succ[i] = next[twin[i]];
        else
            succ[i] = first[t->edges[i]->v2->id];
    }
}

void euler_tour_parallel(graph *t, vector<ll> &succ)
{
    vector<ll> first(t->n, -1), next(t->m, -1), twin(t->m, -1);

#pragma omp parallel for num_threads(THREADS)
    for (ll x = 0; x < ceil(t->m, PER_THREAD / 10); x++)
    {
        for (ll j = 0; j < PER_THREAD / 10; j++)
        {
            ll i = PER_THREAD / 10 * x + j;
            if (i >= t->m)
                break;
            edge *rev = reverseEdge(t->edges[i]);
            twin[distance(t->edges.begin(), lower_bound(t->edges.begin(), t->edges.end(), rev, cmp))] = i;
            delete (rev);

            if (i == 0 || t->edges[i]->v1->id != t->edges[i - 1]->v1->id)
                first[t->edges[i]->v1->id] = i;
            else if (t->edges[i]->v1->id == t->edges[i - 1]->v1->id)
                next[i - 1] = i;
        }
    }
    succ.resize(t->m, -1);

#pragma omp parallel for num_threads(THREADS)
    for (ll x = 0; x < ceil(t->m, PER_THREAD / 10); x++)
    {
        for (ll j = 0; j < PER_THREAD / 10; j++)
        {
            ll i = PER_THREAD / 10 * x + j;
            if (i >= t->m)
                break;
            if (next[twin[i]] != -1)
                succ[i] = next[twin[i]];
            else
                succ[i] = first[t->edges[i]->v2->id];
        }
    }
}

void preOrderVertices(graph *t, vector<ll> &pre, vector<ll> &succ)
{
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

void find_low_parallel(graph *t, graph *nt, vector<ll> &low, vector<ll> &level)
{
    low.resize(nt->n);

#pragma omp parallel for num_threads(THREADS)
    for (ll i = 0; i < ceil(nt->n, PER_THREAD); i++)
        for (ll j = 0; j < PER_THREAD; j++)
        {
            if (i >= nt->n)
                break;
            low[i] = i;
        }

    bool changed = true;
    while (changed)
    {
        changed = false;

#pragma omp parallel for num_threads(THREADS)
        for (ll y = 0; y < ceil(nt->n, PER_THREAD); y++)
        {
            for (ll x = 0; x < PER_THREAD; x++)
            {
                ll i = PER_THREAD * y + x;
                if (i >= nt->n)
                    break;

                for (edge *e : t->vertices[i]->edges)
                {

                    ll j = e->v1->id == i ? e->v2->id : e->v1->id;

                    if (level[i] < level[j] && level[low[j]] < level[low[i]])
                    {
                        low[i] = low[j];
                        changed = true;
                    }
                }

                for (edge *e : nt->vertices[i]->edges)
                {

                    ll j = e->v1->id == i ? e->v2->id : e->v1->id;

                    if (level[low[j]] < level[low[i]])
                    {
                        low[i] = low[j];
                        changed = true;
                    }
                }
            }
        }
    }
}

void find_low(graph *t, graph *nt, vector<ll> &low, vector<ll> &level)
{
    low.resize(nt->n);

    for (ll i = 0; i < nt->n; i++)
        low[i] = i;

    bool changed = true;
    while (changed)
    {
        changed = false;

        for (ll i = 0; i < nt->n; i++)
        {
            for (edge *e : t->vertices[i]->edges)
            {

                ll j = e->v1->id == i ? e->v2->id : e->v1->id;

                if (level[i] < level[j] && level[low[j]] < level[low[i]])
                {
                    low[i] = low[j];
                    changed = true;
                }
            }

            for (edge *e : nt->vertices[i]->edges)
            {

                ll j = e->v1->id == i ? e->v2->id : e->v1->id;

                if (level[low[j]] < level[low[i]])
                {
                    low[i] = low[j];
                    changed = true;
                }
            }
        }
    }
}

graph *auxillary_graph_parallel(graph *g, graph *t, graph *nt, vector<ll> &low, vector<ll> &level, vector<ll> &parent, vector<ll> &pre)
{
    graph *aux = new graph();
    aux->n = g->n + nt->m; 
    aux->vertices.resize(aux->n);

#pragma omp parallel for num_threads(THREADS)
    for (ll i = 0; i < ceil(aux->n, PER_THREAD); i++)
    {
        for (ll j = 0; j < PER_THREAD; j++)
        {
            ll k = PER_THREAD * i + j;
            if (k >= aux->n)
                break;
            vertex *v = new vertex();
            v->id = k;
            aux->vertices[k] = v;
        }
    }
    vector<ll> num(g->m, 0), pref(g->m, 0);

    vector<bool> isTreeEdge(g->m, false);

#pragma omp parallel for num_threads(THREADS)
    for (ll i = 0; i < ceil(g->m, PER_THREAD); i++)
    {
        for (ll j = 0; j < PER_THREAD; j++)
        {
            ll k = PER_THREAD * i + j;
            if (k >= g->m)
                break;
            if (findEdgeById(t->edges, g->edges[k]))
            {
                isTreeEdge[k] = true;
            }
            else
            {
                num[k] = 1;
            }
        }
    }

    partial_sum(num.begin(), num.end(), pref.begin());

#pragma omp parallel num_threads(THREADS)
    {
        vector<edge *> edges;
        vector<vector<edge *>> vertex_edges(aux->n, vector<edge *>(0));

#pragma omp for
        for (ll k = 0; k < ceil(g->m, PER_THREAD); k++)
        {
            for (ll j = 0; j < PER_THREAD; j++)
            {
                ll i = PER_THREAD * k + j;
                if (i >= g->m)
                    break;
                if (isTreeEdge[i])
                {
                    ll u = g->edges[i]->v1->id, v = g->edges[i]->v2->id;
                    if (level[u] < level[v] && level[low[v]] <= level[u])
                    { // CASE 3
                        edge *e = new edge();
                        e->v1 = aux->vertices[u];
                        e->v2 = aux->vertices[v];

                        edges.push_back(e);

                        vertex_edges[u].push_back(e);
                        vertex_edges[v].push_back(e);
                    }
                }
                else
                {
                    ll u = g->edges[i]->v1->id, v = g->edges[i]->v2->id;
                    ll p = LCA(level, parent, u, v);
                    if (pre[v] < pre[u])
                    { // CASE 1
                        edge *e = new edge();
                        e->v1 = aux->vertices[u];
                        e->v2 = aux->vertices[pref[i] + g->n - 1];

                        edges.push_back(e);

                        vertex_edges[u].push_back(e);
                        vertex_edges[pref[i] + g->n - 1].push_back(e);
                    }
                    if (u < v && p != u && p != v)
                    { // CASE 2
                        edge *e = new edge();
                        e->v1 = aux->vertices[u];
                        e->v2 = aux->vertices[v];

                        edges.push_back(e);

                        vertex_edges[u].push_back(e);
                        vertex_edges[v].push_back(e);
                    }
                }
            }
        }

#pragma omp critical
        {
            aux->edges.insert(aux->edges.end(), edges.begin(), edges.end());

            for (ll i = 0; i < aux->n; i++)
            {
                aux->vertices[i]->edges.insert(aux->vertices[i]->edges.end(), vertex_edges[i].begin(), vertex_edges[i].end());
            }
        }
    }

    return aux;
}

void remap_aux_graph_parallel(graph *t, vector<set<ll>> &bi, vector<vector<ll>> &components, vector<ll> &parent)
{
    bi.resize(components.size() - 1);
#pragma omp parallel num_threads(THREADS)
    {
        vector<set<ll>> bi_private(components.size() - 1);
#pragma omp for
        for (ll k = 1; k < ceil(components.size(), PER_THREAD); k++)
        {
            for (ll x = 0; x < PER_THREAD; x++)
            {
                ll i = PER_THREAD * k + x;
                if (i >= components.size())
                    break;
                for (ll j = 0; j < components[i].size(); j++)
                {
                    if (components[i][j] < t->n)
                    {
                        bi_private[i - 1].insert(components[i][j]);
                        bi_private[i - 1].insert(parent[components[i][j]]);
                    }
                }
            }
        }

#pragma omp critical
        {
            for (ll i = 0; i < bi_private.size(); i++)
            {
                bi[i].insert(bi_private[i].begin(), bi_private[i].end());
            }
        }
        for (ll i = 0; i < bi_private.size(); i++)
            bi_private[i].clear();
        bi_private.clear();
    }
}

graph *auxillary_graph(graph *g, graph *t, graph *nt, vector<ll> &low, vector<ll> &level, vector<ll> &parent, vector<ll> &pre)
{
    graph *aux = new graph();
    aux->n = g->n + nt->m;
    for (ll i = 0; i < aux->n; i++)
    {
        vertex *v = new vertex();
        v->id = i;
        aux->vertices.push_back(v);
    }
    vector<ll> num(g->m, 0), pref(g->m, 0);
    ll nti = 0, ti = 0;
    for (ll i = 0; i < g->m; i++)
    {
        if (nti >= nt->m || (ti < t->m && g->edges[i]->v1->id == t->edges[ti]->v1->id && g->edges[i]->v2->id == t->edges[ti]->v2->id))
            ti++;
        else
        {
            num[i] = 1;
            nti++;
        }
    }

    pref[0] = num[0];
    for (ll i = 1; i < g->m; i++)
        pref[i] = pref[i - 1] + num[i];

    ti = 0;
    nti = 0;
    for (ll i = 0; i < g->m; i++)
    {
        ll u = g->edges[i]->v1->id, v = g->edges[i]->v2->id;
        if (nti >= nt->edges.size() || (ti < t->m && g->edges[i]->v1 == t->edges[ti]->v1 && g->edges[i]->v2 == t->edges[ti]->v2))
        {
            if (level[u] < level[v] && level[low[v]] <= level[u])
            {
                edge *e = new edge();
                e->v1 = aux->vertices[u];
                e->v2 = aux->vertices[v];
                aux->edges.push_back(e);
                aux->vertices[u]->edges.push_back(e);
                aux->vertices[v]->edges.push_back(e);
            }
            ti++;
        }
        else
        {
            ll p = LCA(level, parent, u, v);
            if (pre[v] < pre[u])
            {
                edge *e = new edge();
                e->v1 = aux->vertices[u];
                e->v2 = aux->vertices[pref[i] + g->n - 1];
                aux->edges.push_back(e);
                aux->vertices[u]->edges.push_back(e);
                aux->vertices[pref[i] + g->n - 1]->edges.push_back(e);
            }
            if (u < v && p != u && p != v)
            {
                edge *e = new edge();
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

graph *auxillary_graph_uf(graph *g, graph *t, graph *nt, vector<ll> &low, vector<ll> &level, vector<ll> &parent, vector<ll> &pre)
{
    graph *aux = new graph();
    aux->n = g->n + nt->m;

    UnionFind *union_find = new UnionFind(aux->n);

    for (ll i = 0; i < aux->n; i++)
    {
        vertex *v = new vertex();
        v->id = i;
        aux->vertices.push_back(v);
    }
    vector<ll> num(g->m, 0), pref(g->m, 0);
    ll nti = 0, ti = 0;
    for (ll i = 0; i < g->m; i++)
    {
        if (nti >= nt->m || (ti < t->m && g->edges[i]->v1->id == t->edges[ti]->v1->id && g->edges[i]->v2->id == t->edges[ti]->v2->id))
            ti++;
        else
        {
            num[i] = 1;
            nti++;
        }
    }

    pref[0] = num[0];
    for (ll i = 1; i < g->m; i++)
        pref[i] = pref[i - 1] + num[i];

    ti = 0;
    nti = 0;
    for (ll i = 0; i < g->m; i++)
    {
        ll u = g->edges[i]->v1->id, v = g->edges[i]->v2->id;
        if (nti >= nt->edges.size() || (ti < t->m && g->edges[i]->v1 == t->edges[ti]->v1 && g->edges[i]->v2 == t->edges[ti]->v2))
        {
            if (level[u] < level[v] && level[low[v]] <= level[u])
            {
                edge *e = new edge();
                e->v1 = aux->vertices[u];
                e->v2 = aux->vertices[v];
                if (union_find->find(u) != union_find->find(v))
                {
                    union_find->unite(u, v);
                    aux->edges.push_back(e);
                    aux->vertices[u]->edges.push_back(e);
                    aux->vertices[v]->edges.push_back(e);
                }
            }
            ti++;
        }
        else
        {
            ll p = LCA(level, parent, u, v);
            if (pre[v] < pre[u])
            {
                edge *e = new edge();
                e->v1 = aux->vertices[u];
                e->v2 = aux->vertices[pref[i] + g->n - 1];
                if (union_find->find(u) != union_find->find(pref[i] + g->n - 1))
                {
                    union_find->unite(u, pref[i] + g->n - 1);
                    aux->edges.push_back(e);
                    aux->vertices[u]->edges.push_back(e);
                    aux->vertices[pref[i] + g->n - 1]->edges.push_back(e);
                }
            }
            if (u < v && p != u && p != v)
            {
                edge *e = new edge();
                e->v1 = aux->vertices[u];
                e->v2 = aux->vertices[v];
                if (union_find->find(u) != union_find->find(v))
                {
                    union_find->unite(u, v);
                    aux->edges.push_back(e);
                    aux->vertices[u]->edges.push_back(e);
                    aux->vertices[v]->edges.push_back(e);
                }
            }
            nti++;
        }
    }

    delete (union_find);

    return aux;
}

graph *auxillary_graph_parallel_uf(graph *g, graph *t, graph *nt, vector<ll> &low, vector<ll> &level, vector<ll> &parent, vector<ll> &pre)
{
    graph *aux = new graph();
    aux->n = g->n + nt->m;
    aux->vertices.resize(aux->n);
    UnionFind *union_find = new UnionFind(aux->n);

#pragma omp parallel for num_threads(THREADS)
    for (ll i = 0; i < ceil(aux->n, PER_THREAD); i++)
    {
        for (ll j = 0; j < PER_THREAD; j++)
        {
            ll k = PER_THREAD * i + j;
            if (k >= aux->n)
                break;
            vertex *v = new vertex();
            v->id = k;
            aux->vertices[k] = v;
        }
    }
    vector<ll> num(g->m, 0), pref(g->m, 0);

    vector<bool> isTreeEdge(g->m, false);

#pragma omp parallel for num_threads(THREADS)
    for (ll i = 0; i < ceil(g->m, PER_THREAD); i++)
    {
        for (ll j = 0; j < PER_THREAD; j++)
        {
            ll k = PER_THREAD * i + j;
            if (k >= g->m)
                break;
            if (findEdgeById(t->edges, g->edges[k]))
            {
                isTreeEdge[k] = true;
            }
            else
            {
                num[k] = 1;
            }
        }
    }

    partial_sum(num.begin(), num.end(), pref.begin());

#pragma omp parallel num_threads(THREADS)
    {
        vector<edge *> edges;
        vector<vector<edge *>> vertex_edges(aux->n, vector<edge *>(0));

#pragma omp for
        for (ll k = 0; k < ceil(g->m, PER_THREAD); k++)
        {
            for (ll j = 0; j < PER_THREAD; j++)
            {
                ll i = PER_THREAD * k + j;
                if (i >= g->m)
                    break;
                if (isTreeEdge[i])
                {
                    ll u = g->edges[i]->v1->id, v = g->edges[i]->v2->id;
                    if (level[u] < level[v] && level[low[v]] <= level[u])
                    { // CASE 3
                        edge *e = new edge();
                        e->v1 = aux->vertices[u];
                        e->v2 = aux->vertices[v];

                        if (union_find->find(u) != union_find->find(v))
                        {
                            union_find->unite(u, v);
                            edges.push_back(e);

                            vertex_edges[u].push_back(e);
                            vertex_edges[v].push_back(e);
                        }
                    }
                }
                else
                {
                    ll u = g->edges[i]->v1->id, v = g->edges[i]->v2->id;
                    ll p = LCA(level, parent, u, v);
                    if (pre[v] < pre[u])
                    { // CASE 1
                        edge *e = new edge();
                        e->v1 = aux->vertices[u];
                        e->v2 = aux->vertices[pref[i] + g->n - 1];

                        if (union_find->find(u) != union_find->find(pref[i] + g->n - 1))
                        {
                            union_find->unite(u, pref[i] + g->n - 1);
                            edges.push_back(e);

                            vertex_edges[u].push_back(e);
                            vertex_edges[pref[i] + g->n - 1].push_back(e);
                        }
                    }
                    if (u < v && p != u && p != v)
                    { // CASE 2
                        edge *e = new edge();
                        e->v1 = aux->vertices[u];
                        e->v2 = aux->vertices[v];

                        if (union_find->find(u) != union_find->find(v))
                        {
                            union_find->unite(u, v);
                            edges.push_back(e);

                            vertex_edges[u].push_back(e);
                            vertex_edges[v].push_back(e);
                        }
                    }
                }
            }
        }

#pragma omp critical
        {
            aux->edges.insert(aux->edges.end(), edges.begin(), edges.end());

            for (ll i = 0; i < aux->n; i++)
            {
                aux->vertices[i]->edges.insert(aux->vertices[i]->edges.end(), vertex_edges[i].begin(), vertex_edges[i].end());
            }
        }
    }

    delete (union_find);

    return aux;
}

void remap_aux_graph(graph *t, vector<set<ll>> &bi, vector<vector<ll>> &components, vector<ll> &parent)
{
    bi.resize(components.size() - 1);
    for (ll i = 1; i < components.size(); i++)
    {
        for (ll j = 0; j < components[i].size(); j++)
        {
            if (components[i][j] < t->n)
            {
                bi[i - 1].insert(components[i][j]);
                bi[i - 1].insert(parent[components[i][j]]);
            }
        }
    }
}