#include <queue>
#include "timer.hpp"

void Timer::compute_slack() {
    // TO IMPLEMENT
    // topo sort
    // propagate arrival times
    at_early.resize(n_nodes, std::numeric_limits<timing_t>::max());
    at_late.resize(n_nodes, std::numeric_limits<timing_t>::lowest());
    std::queue<int>  que;
    std::vector<int> rev_counts(n_nodes, 0);
    for (int i = 0; i < n_nodes; i++) { rev_counts[i] = nodes[i].rev_edges.size(); }
    for (auto &edge : pi_ats) {
        auto &to     = edge.to;
        at_early[to] = edge.early;
        at_late[to]  = edge.late;
        que.push(to);
    }
    while (!que.empty()) {
        auto from = que.front();
        que.pop();
        for (auto &edge : nodes[from].edges) {
            auto &to     = edge.to;
            auto &early  = edge.early;
            auto &late   = edge.late;
            at_early[to] = std::min(at_early[to], at_early[from] + early);
            at_late[to]  = std::max(at_late[to], at_late[from] + late);
            rev_counts[to]--;
            if (rev_counts[to] == 0) { que.push(to); }
        }
    }
    // compute slacks
    for (auto &cst : setup_constraints) {
        auto & ff_clk  = cst.ff_clk;
        auto & ff_data = cst.ff_data;
        auto & value   = cst.value;
        double slack   = clock_period + at_early[ff_clk] - value - at_late[ff_data];
        slack_setup    = std::min(slack_setup, slack);
    }
    for (auto &cst : hold_constraints) {
        auto &ff_clk  = cst.ff_clk;
        auto &ff_data = cst.ff_data;
        auto &value   = cst.value;
        double slack = at_early[ff_data] - at_late[ff_clk] - value;
        slack_hold = std::min(slack_hold, slack);
    }
}
