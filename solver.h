#ifndef SOLVER_H
#define SOLVER_H

#include <ctime>
#include "graph.h"

struct sol_t {
    double dual, prim;
    tree_t t;
};

/* solver returns sol_t given a complete graph, degree restrictions in degm, and end clock time. */
typedef sol_t (*solver_t)(const graph_t&, int *degm, clock_t);

/* lagrangean relaxation with heuristics */
sol_t lang(const graph_t&, int*, clock_t);

/* simulated annealing */
sol_t anneal(const graph_t&, int*, clock_t);

inline sol_t trivial(const graph_t& g, int *degm, clock_t max) {
    sol_t s;

    int n = g.n;

    if (!max || !degm) {}

    s.dual = 0;
    s.t = make_path(n);

    s.prim = s.t.weight(g);
    return s;
}

extern solver_t solvers[3];

#endif
