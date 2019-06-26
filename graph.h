#ifndef GRAPH_H
#define GRAPH_H

#include <cstdlib>

#define GET_W(g, i, j) ((g).wm)[(j) + (i)*((g).n)]

struct graph_t;

struct edge {
    int u, v;
    graph_t *g;
};

struct graph_t {
    int n; /* number of vertices */
    int m; /* number of edges */
    double *wm; /* 2d matrix as array in row-order */
    int alloc;

    graph_t() : graph_t(0, 0) {}

    graph_t(int n, int m) {
        this->n = n;
        this->m = m;
        alloc = 0;
        wm = (double*)malloc(sizeof(double)*n*n);
        if (!wm) return;
        
        alloc = 1;
    }

    graph_t(const graph_t& g) {
        n = g.n;
        m = g.m;
        alloc = 0;
        wm = NULL;

        if (!g) return;

        wm = (double*)malloc(sizeof(double)*n*n);
        if (!wm) return;

        for (int i = 0; i < n*n; i++)
            wm[i] = g.wm[i];

        alloc = 1;
    }

    graph_t& operator=(const graph_t& g) {
        if (wm) { free(wm); wm = NULL; }

        n = g.n;
        m = g.m;
        alloc = 0;
        wm = NULL;
        if (!g) return *this;

        wm = (double*)malloc(sizeof(double)*n*n);
        if(!wm) return *this;

        for (int i = 0; i < n*n; i++)
            wm[i] = g.wm[i];
        
        return *this;
    }

    operator bool() const { return alloc; }

    bool operator !() const { return !alloc; }

    ~graph_t() {
        alloc = 0;
        if (wm) { free(wm); wm = NULL; }
    }
};

inline bool operator< (const edge& ths, const edge& that) {
    return GET_W(*(ths.g), ths.u, ths.v) < GET_W(*(ths.g), that.u, that.v);
}

struct tree_t {
    int n; /* number of vertices (0 ... n-1) */
    int *par; /* parent node, -1 for root */
    int *deg; /* degree count */
    int alloc;

    tree_t() : tree_t(0) {}

    explicit tree_t(int n) {
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

    tree_t& operator=(const tree_t& t) {
        if (t.n != n || !par || !deg) { 
            if (par) { free(par); par = NULL; }
            if (deg) { free(deg); deg = NULL; }
        
            n = t.n;
            alloc = 0;
            if (!t) return *this;
            par = (int*)malloc(sizeof(int)*n);

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

    operator bool() const { return alloc; }

    bool operator !() const { return !alloc; }

    ~tree_t() {
        alloc = 0;
        if (par) { free(par); par = NULL; }
        if (deg) { free(deg); deg = NULL; }
    }

    double weight(const graph_t& g) const {
        double s = 0;
        for (int i = 0; i < n; i++) {
            if (par[i] == -1) continue;
            s += GET_W(g, i, par[i]);
        }
        return s;
    }

    double weight_x(const graph_t& g, int *degm, double h) const {
        double s = 0, hi;
        for (int i = 0; i < n; i++) {
            if (deg[i] > degm[i]) {
                hi = deg[i] - degm[i];
                s += h*hi*hi;
            }
            if (par[i] == -1) continue;
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
