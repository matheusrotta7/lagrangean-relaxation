#ifndef LOCAL_H
#define LOCAL_H

#include "graph.h"

/* arrange neighbor explorations for tree of n vertices. */
int local_init(int n);

/* find a random neighbor of the tree. */
void random_from(tree_t& tn, const tree_t& t);

/* find a random neighbor of the tree with maximum degree constraints. */
void random_from_degm(tree_t& tn, const tree_t& t);

/* find a better neighbor of the tree, may not improve. */
void improve_from(tree_t& tn, const tree_t& t, const graph_t& g);

/* find a better neighbor of the tree with maximum degree constraints,
 * may not improve. */
void improve_from_degm(tree_t& tn, const tree_t& t, const graph_t& g, int *degm);

/* find a best neighbor of the tree, may not improve. */
void best_from(tree_t& tn, const tree_t& t, const graph_t& g);

/* find a best neighbor of the tree with maximum degree constraints,
 * may not improve. */
void best_from_degm(tree_t& tn, const tree_t& t, const graph_t& g, int *degm);

#endif
