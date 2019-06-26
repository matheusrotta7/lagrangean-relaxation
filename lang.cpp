#include "solver.h"
#include "local.h"

static double *_pkey;
static int *_pst;

void prim(tree_t& t, const graph_t& g) {
    int n = g.n;

    double maxw = 0, w;
    for (int i = 0; i < n; i++)
        for (int j = i+1; j < n; j++) {
            w = GET_W(g, i, j);
            if (w > maxw) maxw = w;
        }
    
    for (int i = 0; i < n; i++) {
        _pkey[i] = maxw+1;
        _pst[i] = 0;
        t.deg[i] = 0;
        t.par[i] = -1;
    }
    
    for (int i = 0; i < n; i++) {
        int v = -1;
        double k = maxw + 2, ku;
        for (int u = 0; u < n; u++) {
            ku = _pkey[u];
            if (!_pst[u] && ku < k) {
                v = u;
                k = ku;          
            }
        }

        _pst[v] = 1;

        for (int w = 0; w < n; w++) {
            if (_pst[w]) continue;
            double cvw = GET_W(g, v, w);
            if (cvw < _pkey[w]) {
                _pkey[w] = cvw;
                t.par[w] = v;            
            }
        }
    }

    for (int i = 1; i < n; i++) {
        int u = i, v = t.par[i];
        t.deg[u]++;
        t.deg[v]++;   
    }
}

sol_t lang(const graph_t& g, int *degm, clock_t max) {
    int n = g.n;

    _pkey = (double*)malloc(sizeof(double)*n);
    _pst = (int*)malloc(sizeof(int)*n);
    local_init(n);

    tree_t ubt(n);
    graph_t gu(g);

    prim(ubt, gu);

    fixdeg(ubt, degm);

    sol_t s;
    s.dual = 0;
    s.t = ubt;
    s.prim = ubt.weight(g);

    int *u = (int*)malloc(sizeof(int)*n);
    for (int i = 0; i < n; i++)
        u[i] = 1;

    tree_t st(n);
    double step = 0, dual, alpha = 2, subg2sum = 0;
    int not_improve_dual = 0;

    int degf = 0;
    for (int i = 0; i < n; i++)
        if (degm[i] > degf) degf = degm[i];
    degf = degf*degf;
    
    while (clock() <= max) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i != j)
                    GET_W(gu, i, j) = GET_W(g, i, j) + u[i] + u[j];

        prim(st, gu);

        ubt = st; /* copy */
        fixdeg(ubt, degm);

        double wubt = ubt.weight(g);
        if (wubt < s.prim) {
            s.t = ubt;
            s.prim = wubt;
            not_improve_dual = 0;     
        } else {
            not_improve_dual++;
        }

        if (s.prim - s.dual < 1) {
            s.dual = s.prim;
            break;        
        }

        dual = st.weight(gu);
        subg2sum = 0;
        for (int i = 0; i < n; i++) {
            /* calculate factor for step */
            int degmi = degm[i];
            int gi = st.deg[i] - degmi;
            if (gi > 0 && u[i] == 0) gi = 0;
            subg2sum += (gi*gi)/((float)degf);

            dual -= u[i]*degmi;
        }
        if (s.dual < dual) s.dual = dual;

        if (subg2sum == 0) {
            double stw = st.weight(g);
            s.dual = stw;
            break;        
        }
                    
        for (int i = 0; i < n; i++) {
            u[i] += step*(st.deg[i] - degm[i]);
            if (u[i] < 0) u[i] = 0;
        }

        if (not_improve_dual >= 30) {
            not_improve_dual = 0;
            alpha /= 2;        
        }

        step = alpha*(1.03*s.prim-dual)/subg2sum;
    }

    local_finish();
    free(_pkey);
    free(_pst);
    free(u);

    return s;
}
