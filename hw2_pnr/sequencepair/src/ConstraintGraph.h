#ifndef SP_CONSTRAINT_GRAPH_H_
#define SP_CONSTRAINT_GRAPH_H_

#include "global.h"
#include "SequencePair.h"

PROJECT_NAMESPACE_BEGIN

class PolygonVertex {
    public:
        PolygonVertex(int w, int l) : width_(w), length_(l) {
            horizontal_indegrees_ = 0;
            vertical_indegrees_ = 0;
            x_left_ = 0;
            y_bottom_ = 0;
        }

        /* getter */
        int horizontal_indegrees() { return horizontal_indegrees_; }
        int vertical_indegrees() { return vertical_indegrees_; }
        int x_left() { return x_left_; }
        int y_bottom() { return y_bottom_; }
        int width() { return width_; }
        int length() { return length_; }
        const std::vector<int>& horizontal_out_vertices() { 
            return horizontal_out_vertices_; 
        }
        const std::vector<int>& vertical_out_vertices() { 
            return vertical_out_vertices_; 
        }

        /* setter */
        void incr_horizontal_indegrees() { ++ horizontal_indegrees_; }
        void decr_horizontal_indegrees() { -- horizontal_indegrees_; }
        void incr_vertical_indegrees() { ++ vertical_indegrees_; }
        void decr_vertical_indegrees() { -- vertical_indegrees_; }
        void set_x_left(int x) { x_left_ = x; }
        void set_y_bottom(int y) { y_bottom_ = y; }
        void set_width(int w) { width_ = w; }
        void set_length(int l) { length_ = l; }
        void add_horizontal_out_vertex(int idx) { 
            horizontal_out_vertices_.emplace_back(idx); 
        }
        void add_vertical_out_vertex(int idx) { 
            vertical_out_vertices_.emplace_back(idx); 
        }
    private:
        /* horizontal & vertical constraints */
        int horizontal_indegrees_;
        int vertical_indegrees_;
        /* target vertices of out edges */
        std::vector<int> horizontal_out_vertices_;
        std::vector<int> vertical_out_vertices_;

        /* polygon geometry information */
        int x_left_;
        int y_bottom_;
        int width_;
        int length_;
};

class ConstraintGraph {
    public:
        ConstraintGraph() {}

        /* construct from sequence pair */
        void construct(const SequencePair& sp);
        /* realize floorplan */
        void floorplan();

        /* getter */
        const std::vector<PolygonVertex>& vertices() { return vertices_; }

        /* setter */
        void add_vertex(const PolygonVertex& v) {
            vertices_.emplace_back(v);
        }
    private:
        std::vector<PolygonVertex> vertices_;
};

PROJECT_NAMESPACE_END

#endif  // SP_CONSTRAINT_GRAPH_H_

