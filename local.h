#ifndef LOCAL_H
#define LOCAL_H

#include "graph.h"

extern int *_lpar;
extern int *_lrank;

/* arrange neighbouring modifications for tree of n vertices. */
int local_init(int n);

/* finish use of neighbouring modifications */
void local_finish();

/* find a random neighbor of the tree. */
void random_from(tree_t& tn, const tree_t& t);

/* find a random neighbor of the tree with maximum degree constraints. */
void random_from_degm(tree_t& tn, const tree_t& t);

/* find a better neighbor of the tree, may not improve. */
void improve_from(tree_t& tn, const tree_t& t, const graph_t& g);

/* find a better neighbor of the tree with maximum degree constraints,
 * may not improve. */
void improve_from_degm(tree_t& tn, const tree_t& t, const graph_t& g, int *degm);

/* force tree to follow restrictions by neighbouring modifications. */
void fixdeg(tree_t& t, int *degm);

/* for calculating components in tree minus one edge. */
void _lcomps(const tree_t& t);

#endif
