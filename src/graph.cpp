//
// Created by natalia on 20.09.23.
//

#include "graph.h"

void graph::add_clause(weak_ptr<clause> &cl) {
    auto shared_cl = cl.lock();
    lit lit1 = shared_cl->get_binary(true);
    lit lit2 = shared_cl->get_binary(false);

    if (lit1 == lit{0, false}) {
        logger::log(logger::ERROR, "Non-binary clause seen as binary.");
    }

    add_edge(lit1.neg_copy(), lit2);
    add_edge(lit2.neg_copy(), lit1);
}

void graph::add_edge(lit lit1, lit lit2) {
    if (out_edges.find(lit1) == out_edges.end()) {
        out_edges.emplace(lit1, vector<lit>());
        vertices.insert(lit1);
    }
    out_edges.find(lit1)->second.push_back(lit2);

    if (in_edges.find(lit2) == in_edges.end()) {
        in_edges.emplace(lit2, vector<lit>());
        vertices.insert(lit2);
    }
    in_edges.find(lit2)->second.push_back(lit1);
}

vector<vector<lit>> graph::find_sccs() {
    reset_tarjan();
    unvisited = {vertices};

    while (!unvisited.empty()) {
        lit l0 = *unvisited.begin();
        tarjan(l0);
    }

    return result;
}

void graph::reset_tarjan() {
    ids = {};
    lows = {};
    on_stack.resize(vertices.size());
    std::fill(on_stack.begin(), on_stack.end(), false);
    stack = {};
    maxdf = 0;
    result = {};
}

void graph::tarjan(lit l0) {
    ids.emplace(l0, maxdf);
    lows.emplace(l0, maxdf);
    maxdf++;

    stack.push(l0);
    on_stack[ids[l0]] = true;
    unvisited.erase(l0);

    for (auto &l : out_edges[l0]) {
        if (unvisited.find(l) != unvisited.end()) {
            tarjan(l);
            lows[l0] =  min(lows[l0], lows[l]);
        } else if (on_stack[ids[l]]) {
            lows[l0] = min(lows[l0], ids[l]);
        }
    }

    if (lows[l0] == ids[l0]) {
        vector<lit> scc = {};
        lit pop{0, false};
        do {
            pop = stack.top();
            stack.pop();
            on_stack[ids[pop]] = false;
            scc.push_back(pop);
        } while (!(pop == l0));
        if (scc.size() > 1) {
            result.push_back(scc);
        }
    }
}

set<lit> graph::find_roots() {
    set<lit> roots{};
    for (auto &it : vertices) {
        if (in_edges.find(it) == in_edges.end()) {
            roots.insert(it);
        }
    }
    return roots;
}
