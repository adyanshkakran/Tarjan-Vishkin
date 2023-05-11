#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <set>
#include "graph.h"

std::ostream &operator<<(std::ostream &out, const Graph_test &g);
Edge_test reverseEdge(const Edge_test &e);
size_t LCA(const std::vector<size_t> &parent, const std::vector<size_t> &level, size_t u, size_t v, size_t *a = NULL, size_t *b = NULL);
void prefixSum(std::vector<size_t> &v);
void biccToArticulationPoints(const Graph_test &g, const std::vector< std::set<size_t> > &bicc, std::set<size_t> &articulationPoints);

#endif
