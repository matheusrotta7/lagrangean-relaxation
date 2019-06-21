#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "graph.h"
#include "solver.h"

int main(int argc, char **argv) {
    if (argc < 4) {
	    std::cout << "USAGE: dcmstp-solver INST TIME SOLVER\n";
        return 0;
    }

    int soln = argv[3][0]-49;
    if (argv[3][0] == 'm') soln = 2;

    solver_t solv = solvers[soln];

    int secs = atoi(argv[2]);
    clock_t maxclock = clock()+secs*CLOCKS_PER_SEC;
    int n, m;

    std::ifstream file(argv[1]);

    if (!file) { 
        std::cout << "ERROR: input file couldn't be opened.";
        return 0;
    }

    file >> n >> m;

    graph_t g(n, m, 0);
    if (!g) return 0;

    int *degm = (int*)malloc(sizeof(int)*n);

    if (!degm) return 0;

    for (int u = 0; u < n; u++)
        GET_W(g, u, u) = 0;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        file >> u >> v >> w;
        u -= 1;
        v -= 1;

        GET_W(g, u, v) = w;
        GET_W(g, v, u) = w;
    }

    for (int i = 0; i < n; i++) {
        int u, d;
        file >> u >> d;
        degm[u-1] = d;
    }

    sol_t sol = solv(g, degm, maxclock);
    std::cout << std::setprecision(4) << std::fixed;
    std::cout << argv[1] << ',' << sol.dual << ',' << 
        sol.prim << std::endl;

    char *outfn = (char*)malloc(sizeof(char)*(strlen(argv[1])+5));
    strcpy(outfn, argv[1]);
    strcat(outfn, ".out");

    std::ofstream outf(outfn);
    
    for (int i = 1; i < n; i++) {
        int u = i, v = sol.t.par[i];
        if (u > v) {
            u = v;
            v = i;
        }
        outf << (u+1) << ' ' << (v+1) << '\n'; 
    }

    free(degm);
    return 0;
}
