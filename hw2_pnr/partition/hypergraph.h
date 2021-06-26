#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <limits>
#include <queue>
using std::priority_queue;
using std::vector;
using std::shared_ptr;
using std::unordered_map;

using Value_Type = int;

class Node {
public:
    Node() {}
    Node(int blocks) {
        netBelong.resize(0);
        blockBelong = 0;
    }
    void insertNet(int netId) { netBelong.push_back(netId); }
    vector<int>& nets() { return netBelong; }
    int getBlock() { return blockBelong; }
    void setBlock(int block) { blockBelong = block; }

private:
    vector<int> netBelong; // Net ID the node connect to, you may regard each net as a hyperedge
    int blockBelong; // the block the node is divided into
};

class Net {
public:
    enum class Status {
        FREE,
        LOOSE,
        LOCKED
    };

    Net() {}
    Net(int blocks) {
        nodeConnected.resize(0);
    }

    void insertNode(int nodeId) { nodeConnected.push_back(nodeId); }
    vector<int>& nodes() { return nodeConnected; }
    int getNode(int nodeId) {
        if (nodeId > nodeConnected.size()) return -1;
        return nodeConnected[nodeId];
    }
private:
    vector<int> nodeConnected; // the Node ID this net connect
};

class HyperGraph {
public:
    HyperGraph() {}
    HyperGraph(int blockNum) : blocks(blockNum), nodeNum(0),netNum(0) {} 
    Node& addNode() {
        nodes.emplace_back(blocks);
        nodeNum++;
        return nodes[nodeNum - 1];
    }
    Node& getNode(int id) { return nodes[id]; }
    Net& addNet() {
        nets.emplace_back(blocks);
        netNum++;
        return nets[netNum - 1];
    }
    Net& getNet(int id) { return nets[id]; }
    int getNodeNum() { return nodeNum; }
    int getNetNum() { return netNum; }


private:
    int blocks; // How many blocks
    int nodeNum; // how many nodes in this hypergraph
    int netNum; // how many nets in this hyper graph
    vector<Node> nodes; // Node list
    vector<Net> nets; // Net list
};

#endif