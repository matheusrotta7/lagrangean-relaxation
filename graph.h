#ifndef GRAPH_H
#define GRAPH_H

#include <cstdlib>

#define GET_W(g, i, j) ((g).wm)[(j) + (i)*((g).n)]

struct edge {
    int u, v;
};

struct graph_t {
    int n; /* number of vertices */
    int m; /* number of edges */
    int *wm; /* 2d matrix as array in row-order */
    edge *elist; /* possible space for edge list */
    int alloc;

    graph_t() : graph_t(0, 0, 0) {}

    graph_t(int n, int m, int mk_elist) {
        this->n = n;
        this->m = m;
        elist = NULL;
        alloc = 0;
        wm = (int*)malloc(sizeof(int)*n*n);
        if (!wm) return;
        if (!mk_elist) {
            alloc = 1;
            return;
        }
        elist = (edge*)malloc(sizeof(edge)*m); 
        if (!elist) return;
        alloc = 1;
    }

    graph_t(const graph_t& g) {
        n = g.n;
        m = g.m;
        alloc = 0;
        elist = NULL;
        wm = NULL;
        if (!g) return;

        wm = (int*)malloc(sizeof(int)*n*n);
        if (!wm) return;

        for (int i = 0; i < n*n; i++)
            wm[i] = g.wm[i];

        if (!g.elist) {
            alloc = 1;
            return;
        }
        elist = (edge*)malloc(sizeof(edge)*m);
        if (!elist) return;

        for (int i = 0; i < m; i++)
            elist[i] = g.elist[i];
    }

    graph_t(graph_t&& g) {
        n = g.n;
        m = g.m;
        wm = g.wm;
        elist = g.elist;
        alloc = g.alloc;
        g.alloc = 0;
        g.wm = NULL;
        g.elist = NULL;
    }

    graph_t& operator=(const graph_t& g) {
        if (wm) free(wm);
        if (elist) free(elist);

        n = g.n;
        m = g.m;
        alloc = 0;
        wm = NULL;
        elist = NULL;
        if (!g) return *this;

        wm = (int*)malloc(sizeof(int)*n*n);
        if(!wm) return *this;

        for (int i = 0; i < n*n; i++)
            wm[i] = g.wm[i];
        
        if(!g.elist) {
            alloc = 1;
            return *this;
        }

        elist = (edge*)malloc(sizeof(edge)*m);
        if (!elist) return *this;
        
        for (int i = 0; i < m; i++)
            elist[i] = g.elist[i];
        return *this;
    }

    graph_t& operator=(graph_t&& g) {
        if (wm) free(wm);
        if (elist) free(elist);

        n = g.n;
        m = g.m;
        wm = g.wm;
        elist = g.elist;
        alloc = g.alloc;
        g.alloc = 0;
        g.wm = NULL;
        g.elist = NULL;
        return *this;
    }

    operator bool() const { return alloc; }

    bool operator !() const { return !alloc; }

    ~graph_t() {
        if (wm) free(wm);
        if (elist) free(elist);
    }
};

struct tree_t {
    int n; /* number of vertices (0 ... n-1) */
    int *par; /* parent node, -1 for root */
    int *deg; /* degree count */
    int alloc;

    tree_t() : tree_t(0) {}

    tree_t(int n) {
        alloc = 0;
        this->n = n;
        par = (int*)malloc(sizeof(int)*n);
        deg = NULL;
        if (!par) return;
        deg = (int*)malloc(sizeof(int)*n);
        if (!deg) return;
        alloc = 1;
    }

    tree_t(const tree_t& t) {
        n = t.n;
        alloc = 0;
        par = (int*)malloc(sizeof(int)*n);
        deg = NULL;
        if (!par) return;
        deg = (int*)malloc(sizeof(int)*n);
        if (!deg) return;

        for (int i = 0; i < n; i++) {
            par[i] = t.par[i];
            deg[i] = t.deg[i];
        }

        alloc = 1;
    }

    tree_t(tree_t&& t) {
        n = t.n;
        alloc = t.alloc;
        t.alloc = 0;
        par = t.par;
        deg = t.deg;
        t.par = NULL;
        t.deg = NULL;
    }

    tree_t& operator=(const tree_t& t) {
        
        if (t.n != n || !par || !deg) { 
            if (par) free(par);
            if (deg) free(deg);
        
            n = t.n;
            alloc = 0;
            if (!t) return *this;
            par = (int*)malloc(sizeof(int)*n);
            deg = NULL; 
            if (!par) return *this;
            deg = (int*)malloc(sizeof(int)*n);
            if (!deg) return *this;
        }
        
        for (int i = 0; i < n; i++) {
            par[i] = t.par[i];
            deg[i] = t.deg[i];
        }

        alloc = 1;
        return *this; 
    }

    tree_t& operator=(tree_t&& t) {
        if (par) free(par);
        if (deg) free(deg);

        n = t.n;
        alloc = t.alloc;
        t.alloc = 0;
        par = t.par;
        deg = t.deg;
        t.par = NULL;
        t.deg = NULL;

        return *this;
    }

    operator bool() const { return alloc; }

    bool operator !() const { return !alloc; }

    ~tree_t() {
        if (par) free(par);
        if (deg) free(deg);
    }

    int weight(const graph_t& g) const {
        int s = 0;
        for (int i = 1; i < n; i++) {
            s += GET_W(g, i, par[i]);
        }
        return s;
    }

    int weight_x(const graph_t& g, int *degm, int h) const {
        int s = 0, hi;
        if (deg[0] > degm[0]) {
            hi = deg[0] - degm[0];
            s += h*hi*hi;
        }
        for (int i = 1; i < n; i++) {
            if (deg[i] > degm[i]) {
                hi = deg[i] - degm[i];
                s += h*hi*hi;
            }
            s += GET_W(g, i, par[i]);
        }
        return s;
    }
};

inline tree_t make_path(int n) {
    tree_t t(n);

    if (!t) return t;

    for (int i = 0; i < n; i++) {
        t.par[i] = (i-1);
        t.deg[i] = (i == 0 || i == (n-1)) ? 1 : 2;
    }

    return t;
}

#endif
