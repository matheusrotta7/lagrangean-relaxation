#include "local.h"
#include <cstdlib>

int *_lpar = NULL;
int *_lrank = NULL;

int local_init(int n) {
    if (_lpar) free(_lpar);
    if (_lrank) free(_lrank);
    
    srand(170013);

    _lpar = (int*)malloc(sizeof(int)*n);
    if (!_lpar) return 0;
    _lrank = (int*)malloc(sizeof(int)*n);
    if(!_lrank) {
        free(_lpar);
        return 0;   
    }

    return 1;
}

void _lcomps(const tree_t& t) {
    int n = t.n;

    for (int u = 0; u < n; u++) {
        _lpar[u] = u;
        _lrank[u] = 0;    
    }

    for (int u = 1; u < n; u++) {
        int urt = u, vrt = t.par[u], urt0;

        if (vrt == -1) continue;

        while (_lpar[urt] != urt) {
            _lpar[urt] = _lpar[_lpar[urt]];
            urt = _lpar[urt];
            /* no need for recursion */
        }

        while (_lpar[vrt] != vrt) {
            _lpar[vrt] = _lpar[_lpar[vrt]];
            vrt = _lpar[vrt];
        }

        if (_lrank[urt] < _lrank[vrt]) {
            urt0 = urt;
            urt = vrt;
            vrt = urt0;
        }

        _lpar[vrt] = urt;
        if (_lrank[urt] == _lrank[vrt])
            _lrank[urt] += 1;
        
    }
}

void random_from(tree_t& tn, const tree_t& t) {
    int n = t.n;

    for (int i = 0; i < n; i++) {
        tn.par[i] = t.par[i];
        tn.deg[i] = t.deg[i]; 
    }
    
    int x = 1 + rand() % (n-1), y; /* select non-root */

    tn.deg[t.par[x]]--;
    tn.par[x] = -1;

    _lcomps(tn);

    do {
        y = rand() % n;
    } while (y == t.par[x] || _lpar[x] == _lpar[y]);

    tn.par[x] = y;
    tn.deg[y]++;

    return;
}

void random_from_degm(tree_t& tn, const tree_t& t, int *degm) {
    int n = t.n;

    int full = 1;
    for (int i = 0; i < n; i++) {
        tn.par[i] = t.par[i];
        tn.deg[i] = t.deg[i];
        if (t.deg[i] != degm[i]) full = 0;
    }
    if (full) return;

    int x = 1 + rand() % (n-1), y; /* select non-root */

    tn.deg[t.par[x]]--;
    tn.par[x] = -1;

    _lcomps(tn);

    do { 
        y = rand() % n;
    } while (y == t.par[x] || _lpar[x] == _lpar[y]);

    tn.par[x] = y;
    tn.deg[y]++;

    return;
}

void improve_from(tree_t& tn, const tree_t& t, const graph_t& g) {
    int n = t.n;

    for (int i = 0; i < n; i++) {
        tn.par[i] = t.par[i];
        tn.deg[i] = t.deg[i];
    }

    int y;
    for (int x = 1; x < n; x++) {
        tn.deg[t.par[x]]--;
        tn.par[x] = -1;

        _lcomps(tn);

        for (y = 1; y < n; y++) {
            if (y == t.par[x]) continue;
            if (_lpar[x] == _lpar[y]) continue;
            if (GET_W(g, x, t.par[x]) <= GET_W(g, x, y)) continue;
            break;        
        }
        
        if (y < n) {
            tn.par[x] = y;
            tn.deg[y]++;
            return;
        }

        tn.par[x] = t.par[x];
        tn.deg[t.par[x]]++;
    }

    return;
}

void improve_from_degm(tree_t& tn, const tree_t& t, const graph_t& g, int *degm) {
    int n = t.n;

    int full = 1;
    for (int i = 0; i < n; i++) {
        tn.par[i] = t.par[i];
        tn.deg[i] = t.deg[i];
        if (tn.deg[i] != degm[i]) full = 0;
    }
    if (full) return;

    int y;
    for (int x = 1; x < n; x++) {
        tn.deg[t.par[x]]--;
        tn.par[x] = -1;

        _lcomps(tn);

        for (y = 1; y < n; y++) {
            if (y == t.par[x]) continue;
            if (_lpar[x] == _lpar[y]) continue;
            if (GET_W(g, x, t.par[x]) <= GET_W(g, x, y)) continue;
            break;        
        }
        
        if (y < n) {
            tn.par[x] = y;
            tn.deg[y]++;
            return;
        }

        tn.par[x] = t.par[x];
        tn.deg[t.par[x]]++;
    }

    return;
}
