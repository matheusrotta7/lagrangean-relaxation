#include "solver.h"
#include "local.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

int penalties(const tree_t& t, int *degm) {
    int p = 0;
    int n = t.n;
    for (int i = 0; i < n; i++) {
        if (t.deg[i] > degm[i])
            p += t.deg[i];
    }
    return p;
}

sol_t anneal(const graph_t& g, int *degm, clock_t maxclock) {
    std::cout << "init anneal\n";

    int n = g.n;
    tree_t s0 = make_path(n);
    int w_s0 = s0.weight(g), w_sn, wdiff;

    std::cout << "make path 0\n";

    /* parameters */
    int h = 10*w_s0/n;
    double alpha = 0.95;
    double kt = 3*h;

    tree_t s_opt(s0);
    int w_opt = w_s0;

    tree_t sn(n);
    local_init(n);

    int i = 0;
    while (clock() <= maxclock) {
        i += 1;
        std::cout << "it " << i << '\n';

        random_from(sn, s0);
        w_sn = sn.weight_x(g, degm, h);
        wdiff = w_sn - w_s0;

        std::cout << "s0: p " << penalties(s0, degm) << ", w " << w_s0 << '\n';
        std::cout << "sn: p " << penalties(sn, degm) << ", w "  << w_sn << '\n';
        if (wdiff <= 0) {
            w_s0 = w_sn;
            s0 = sn;

            if (wdiff < 0) {
                w_opt = w_sn;
                s_opt = w_sn;
            }
        } else {
            double prob = exp(-(float(wdiff))/kt);
            std::cout << "probability of jump: " << prob << '\n';
            if (rand()/RAND_MAX < prob) {
                s0 = sn;
                std::cout << "jump\n";
            }
        }

        kt *= alpha;
        
        clock_t now = clock(), after = now + CLOCKS_PER_SEC*2;
        while (clock() <= after);
    }

    int p_opt = penalties(s_opt, degm), p_sn;
    while (p_opt) {
        random_from(sn, s_opt);
        p_sn = penalties(sn, degm);
        if (p_sn < p_opt) {
            s_opt = sn;
            p_opt = p_sn;
        }
    }

    sol_t sol;
    sol.dual = 0;
    sol.prim = s_opt.weight(g);
    sol.t = s_opt;

    return sol;
}
