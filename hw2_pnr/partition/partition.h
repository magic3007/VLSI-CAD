#ifndef PARTITION_H
#define PARTITION_H
#include "hypergraph.h"

class Partitioner {
public:
    Partitioner() : blocks(2), graph(2) {}

    HyperGraph& getGraph() { return graph; }

    void run();

    void print_result();

private:
    HyperGraph graph; // input hypergraph
    int blocks; // # blocks the hypergraph is partition to
    vector<int> blockNodeNum; // How many nodes are in each block
};


#endif // PARTITION_H