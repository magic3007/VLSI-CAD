#include "global.h"
#include "SequencePair.h"
#include "ConstraintGraph.h"
#include "parser.h"

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "[ERROR] input file is required\n");
        exit(0);
    }
    PROJECT_NAMESPACE::SequencePair sp;
    PROJECT_NAMESPACE::ConstraintGraph cg;
    PROJECT_NAMESPACE::parser(argv[1], sp, cg);
    cg.construct(sp);
    cg.floorplan();
    PROJECT_NAMESPACE::writer(cg);

    return 0;
}
