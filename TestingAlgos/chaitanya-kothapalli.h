#ifndef CHAITANYA_KOTHAPALLI_H
#define CHAITANYA_KOTHAPALLI_H

#include "bicc.h"

class ChaitanyaKothapalli : public BiCC
{
public:
	ChaitanyaKothapalli(size_t nthreads = 1) : BiCC(nthreads) {}
	virtual const char *name();
	virtual void getBiCC(const Graph_test &g, std::vector< std::set<size_t> > &bicc);
protected:
	void removeBridges(const Graph_test &g, const Graph_test &t, const Graph_test &nt, std::vector<size_t> &parent, std::vector<size_t> &level, std::vector< std::vector<size_t> > &components, std::vector<Edge_test> *bridges = NULL);
	void auxiliaryGraph(const Graph_test &g, const Graph_test &nt, const std::vector<size_t> &parent, const std::vector<size_t> &level, const std::vector<size_t> &component, Graph_test &aux, std::vector<size_t> &alias);
};

#endif
