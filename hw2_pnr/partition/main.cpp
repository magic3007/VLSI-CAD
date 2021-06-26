#include "parser.h"
#include "hypergraph.h"
#include "partition.h"
#include "thirdparty/clipp.h"

int main(int argc, char** argv) {
    using namespace clipp;
    string inputFile;
    int blocks, levels;
    auto cli = (
        (required("-h", "--hgr") & value("input .hgr format file", inputFile))
    );
    parse(argc, argv, cli);
    Partitioner partitioner;
    parse_input(inputFile, partitioner.getGraph());
    partitioner.run();
    partitioner.print_result();
}
