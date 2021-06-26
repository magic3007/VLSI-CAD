#include "partition.h"
#include <iostream>

void Partitioner::run() {
    /* TODO: 2-way FM */
}

/* You are not allowed to modify the following function */
void Partitioner::print_result() {
    for (int i = 0; i < blocks; i++) {
        printf("Block #%d, Node Num: %d\n", i, blockNodeNum[i]);
    }
    Value_Type totalCuts = 0;
    int netNum = graph.getNetNum();
    for (int i = 0; i < netNum; i++) {
        auto &net = graph.getNet(i);
        if (net.nodes().size() == 0) continue;
        Value_Type cuts = -1;
        vector<bool> vis(blocks, 0); 
        for (auto nodeId : net.nodes()) {
            auto& node = graph.getNode(nodeId);
            if (!vis[node.getBlock()]) {
                cuts++;
                vis[node.getBlock()] = 1;
            }
        }
        totalCuts += cuts;
    }
    printf("total Cuts: %d\n", totalCuts);
    Value_Type totalPenalty = std::max(blockNodeNum[0] - blockNodeNum[1], blockNodeNum[1] - blockNodeNum[0]) + totalCuts;
    printf("total Penalty: %d\n", totalPenalty);
}
