//
// Created by natalia on 20.09.23.
//

#ifndef SATSOLVER_GRAPH_H
#define SATSOLVER_GRAPH_H
#include <vector>
#include <map>
#include "lit.h"
#include "clause.h"
#include <stack>
#include <set>


class graph {
private:

    // for tarjan
    int maxdf;
    map<lit, int> ids;
    map<lit, int> lows;
    vector<bool> on_stack;
    stack<lit> s;
    set<lit> unvisited;
    vector<vector<lit>> result;

    void reset_tarjan();
    void tarjan(lit l0);

public:
    set<lit> vertices;
    map<lit, vector<lit>> out_edges;
    map<lit, vector<lit>> in_edges;

    void add_edge(lit lit1, lit lit2);
    void remove_edge(lit lit1, lit lit2);
    void add_clause(weak_ptr<clause> &cl);

    vector<vector<lit>> find_sccs();

    set<lit> find_roots();

    set<lit> get_vertices();

    void remove_clause(lit lit1, lit lit2);
};


#endif //SATSOLVER_GRAPH_H
