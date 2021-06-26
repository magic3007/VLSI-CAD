#ifndef SP_PARSER_H_
#define SP_PARSER_H_

#include "global.h"
#include "SequencePair.h"
#include "ConstraintGraph.h"

PROJECT_NAMESPACE_BEGIN

void parser(const std::string& filename, SequencePair& sp, ConstraintGraph& cg) {
    std::ifstream ifs(filename.c_str(), std::ifstream::in);
    if(!ifs.is_open()) {
        fprintf(stderr, "[ERROR] %s parser cannot open file %s\n", __func__, filename.c_str());
        exit(0);
    }

    std::string buf;
    int num, idx;

    ifs >> num;
    sp.set_num_modules(num);
    for(int i = 0; i < num; i++) {
        ifs >> idx;
        sp.add_pos_seq(idx);
    }
    for(int i = 0; i < num; i++) {
        ifs >> idx;
        sp.add_neg_seq(idx);
    }

    int width, length;
    for(int i = 0; i < num; i++) {
        ifs >> width >> length;
        cg.add_vertex(PolygonVertex(width, length));
    }

    return;
}

void writer(ConstraintGraph& cg) {
    for(auto vertex : cg.vertices()) {
        fprintf(stdout, "%d %d %d %d\n", vertex.x_left(), vertex.y_bottom(), 
                vertex.width(), vertex.length());
    }
    return;
}

PROJECT_NAMESPACE_END

#endif  // SP_PARSER_H_

