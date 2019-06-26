#include "local.h"
#include <cstdlib>
#include <ctime>

int *_lpar = NULL;
int *_lrank = NULL;

int local_init(int n) {
    if (_lpar) free(_lpar);
    if (_lrank) free(_lrank);
    
    srand(time(NULL));

    _lpar = (int*)malloc(sizeof(int)*n);
    if (!_lpar) return 0;
    _lrank = (int*)malloc(sizeof(int)*n);
    if(!_lrank) {
        free(_lpar);
        return 0;
    }

    return 1;
}

void local_finish() {
    if (_lpar) free(_lpar);
    if (_lrank) free(_lrank);
}


#include <iostream>
void fixdeg(tree_t& t, int *degm) {
    int n = t.n;
    for (int i = 0; i < n; i++) {
        while (t.deg[i] > degm[i]) {
            int j, k;
            for (j = 0; j < n; j++)
                if (t.par[j] == i) break;

            t.deg[i]--;
            t.par[j] = -1;

            _lcomps(t);

            for (k = 0; k < n; k++)
                if (_lpar[k] != _lpar[j] && t.deg[k] < degm[k])
                    break;
            if (k == n) {
                t.deg[i]++;
                t.par[j] = i;
                return; /* failure */           
            }

            t.par[j] = k;
            t.deg[k]++;
        }
    }
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

        if (urt == vrt) continue;

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

    int x, y;
    while (1) {
        x = 1 + rand() % (n-1); /* select x non-root */
        y = -1;

        tn.deg[t.par[x]]--;
        tn.par[x] = -1;

        _lcomps(tn);

        do {
            y++;
            if (y == n) break;
        } while (y == t.par[x] || _lpar[x] == _lpar[y]);
        
        if (y < n) {
            tn.par[x] = y;
            tn.deg[y]++;
            break;
       }

       tn.deg[t.par[x]]++;
       tn.par[x] = t.par[x];
    }
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
    } while (y == t.par[x] || _lpar[x] == _lpar[y] || tn.deg[y] == degm[y]);

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
