#include <bits/stdc++.h>

using namespace std;

long long int parent[200001];
long long int rank_[200001];

typedef struct edge {
    long long int u;
    long long int v;
    long long int w;
} edge;

void make_set (long long int x) {
    parent[x] = x;
    rank_[x] = 0;
}

long long int find_set (long long int x) {
    if (x != parent[x])
        parent[x] = find_set (parent[x]);
    return parent[x];
}

void link (long long int x, long long int y) {
    if (rank_[x] > rank_[y])
        parent[y] = x;
    else
        parent[x] = y;
        if (rank_[x] == rank_[y])
            rank_[y]++;
}

void unite (long long int x, long long int y) {
    link(find_set(x), find_set(y));
}

bool comp (edge a, edge b) {
    return a.w < b.w;
}

long long int  mst_kruskal(vector<edge> graph, long long int m, long long int n) {
    long long int  used_weights = 0;
    for (long long int i = 0; i < m; i++) {
        make_set(i);
    }
    sort(graph.begin(), graph.end(), comp);
    for (long long int i = 0; i < n; i++) {
        long long int u, v, w;
        u = graph[i].u;
        v = graph[i].v;
        w = graph[i].w;
        if (find_set(u) != find_set(v)) {
            used_weights += w;
            unite(u, v);
        }
    }
    return used_weights;
}

