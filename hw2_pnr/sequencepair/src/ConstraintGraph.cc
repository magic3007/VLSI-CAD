#include "ConstraintGraph.h"

PROJECT_NAMESPACE_BEGIN

void ConstraintGraph::construct(const SequencePair& sp) {
    /* TODO: construct cg from sp */
    int              num_modules = sp.num_modules();
    auto &           pos_seq     = sp.pos_seq();
    auto &           neg_seq     = sp.neg_seq();
    std::vector<int> neg_seq_idxes(num_modules, 0);
    for (int i = 0; i < num_modules; i++) { neg_seq_idxes[neg_seq[i]] = i; }
    for (int i = 0; i < num_modules; i++) {
        for (int j = i + 1; j < num_modules; j++) {
            int a = pos_seq[i];
            int b = pos_seq[j];
            if (neg_seq_idxes[a] < neg_seq_idxes[b]) {
                // the right boundary of A is on the left hand side of the left boundary of B
                vertices_[a].add_horizontal_out_vertex(b);
                vertices_[b].incr_horizontal_indegrees();
            } else {
                // the upper boundary of B is below the lower boundary of A
                vertices_[a].add_vertical_out_vertex(b);
                vertices_[b].incr_vertical_indegrees();
            }
        }
    }
}

void ConstraintGraph::floorplan() {
    /* TODO: recover floorplan from cg */
    int num_modules = vertices_.size();
    {
        // horizontal graph
        std::priority_queue<std::pair<int, int>> que;
        std::vector<bool>                        visited(num_modules, false);
        for (int i = 0; i < num_modules; i++) {
            if (vertices_[i].horizontal_indegrees() == 0) { que.push(std::make_pair(0, i)); }
        }
        while (!que.empty()) {
            int dist = que.top().first;
            int u    = que.top().second;
            que.pop();
            if (visited[u]) continue;
            visited[u] = true;
            vertices_[u].set_x_left(dist);
            for (auto &v : vertices_[u].horizontal_out_vertices()) {
                que.push(std::make_pair(dist + vertices_[u].width(), v));
            }
        }
    }
    {
        // vertical graph
        std::priority_queue<std::pair<int, int>> que;
        std::vector<bool>                        visited(num_modules, false);
        for (int i = 0; i < num_modules; i++) {
            if (vertices_[i].vertical_indegrees() == 0) { que.push(std::make_pair(0, i)); }
        }
        while (!que.empty()) {
            int dist = que.top().first;
            int u    = que.top().second;
            que.pop();
            visited[u] = true;
            vertices_[u].set_y_bottom(dist);
            for (auto &v : vertices_[u].vertical_out_vertices()) {
                que.push(std::make_pair(dist + vertices_[u].length(), v));
            }
        }
    }
}

PROJECT_NAMESPACE_END
