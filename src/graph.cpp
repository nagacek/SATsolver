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

void graph::remove_clause(lit lit1, lit lit2) {
    remove_edge(lit1.neg_copy(), lit2);
    remove_edge(lit2.neg_copy(), lit1);
}

void graph::remove_edge(lit lit1, lit lit2) {
    auto out1 = out_edges[lit1];
    out1.erase(std::find(out1.begin(), out1.end(), lit2));
    if (out1.empty()) {
        if (in_edges[lit1].empty()) {
            vertices.erase(lit1);
        }
    }
    auto in2 = in_edges[lit2];
    in2.erase(std::find(in2.begin(), in2.end(), lit1));
    if (in2.empty()) {
        if (out_edges[lit2].empty()) {
            vertices.erase(lit2);
        }
    }
}

void graph::add_edge(lit lit1, lit lit2) {
    if (out_edges.find(lit1) == out_edges.end()) {
        out_edges.emplace(lit1, vector<lit>());
        vertices.insert(lit1);
    }
    auto it = std::find(out_edges[lit1].begin(), out_edges[lit1].end(),lit2);
    if (it == out_edges[lit1].end()) {
        out_edges[lit1].push_back(lit2);
    }

    if (in_edges.find(lit2) == in_edges.end()) {
        in_edges.emplace(lit2, vector<lit>());
        vertices.insert(lit2);
    }
    it = std::find(in_edges[lit2].begin(), in_edges[lit2].end(),lit1);
    if (it == in_edges[lit2].end()) {
        in_edges[lit2].push_back(lit1);
    }
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
        if (in_edges.find(it) == in_edges.end() || in_edges.at(it).empty()) {
            roots.insert(it);
        }
    }
    return roots;
}

set<lit> graph::get_vertices() {
    return vertices;
}

