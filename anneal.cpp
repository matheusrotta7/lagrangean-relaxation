#include "solver.h"
#include "local.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

sol_t anneal(const graph_t& g, int *degm, clock_t maxclock) {
    int n = g.n;
    tree_t s0 = make_path(n);
    double w_s0 = s0.weight(g), w_sn, wdiff;

    /* parameters */
    double h = w_s0/n;
    double alpha = 0.99999;
    double kt = 40*h;

    tree_t s_opt(s0);
    double w_opt = w_s0;

    local_init(n);

    s0 = s_opt;
    w_opt = s_opt.weight(g);
    w_s0 = w_opt;

    tree_t sn(n);

    while (clock() <= maxclock) {
        improve_from(sn, s0, g);
        w_sn = sn.weight_x(g, degm, h);
        wdiff = w_sn - w_s0;

        if (wdiff <= 0) {
            w_s0 = w_sn;
            s0 = sn;
            if (w_sn < w_opt) {
                w_opt = w_sn;
                s_opt = sn;
            }
        } else {
            double prob = exp(-(float(wdiff))/kt);
            if (float(rand())/RAND_MAX < prob) {
                w_s0 = w_sn;
                s0 = sn;
            }
        }
        if (kt > 1 && kt*alpha <= 1) {
            std::cout << "freeze\n";        
        }
        if (kt > 1) kt *= alpha;
    }

    fixdeg(s_opt, degm);

    local_finish();

    sol_t sol;
    sol.dual = 0;
    sol.prim = s_opt.weight(g);
    sol.t = s_opt;

    return sol;
}
