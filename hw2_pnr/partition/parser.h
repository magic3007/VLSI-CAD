#ifndef HYPERGRAPH_PARSER_H
#define HYPERGRAPH_PARSER_H
#include "hypergraph.h"

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using std::string;
using std::unordered_map;
using std::ifstream;
using std::stringstream;
using std::getline;

void parse_input(string fileName, HyperGraph& graph) {
    ifstream is(fileName);
    int nodeNum, netNum;
    is >> netNum >> nodeNum;
    for (int i = 0; i < nodeNum; i++)
        graph.addNode();
    string s;
    getline(is, s);
    for (int i = 0; i < netNum; i++) {
        getline(is, s);
        Net& net = graph.addNet();
        int len = s.size();
        int now = 0;
        bool flag = 0;
        for (int j = 0; j < len; j++) {
            if (s[j] == ' ' && flag) {
                now--;
                net.insertNode(now);
                graph.getNode(now).insertNet(i);
                now = 0;
                flag = 0;
            }
            else if (s[j] >= '0' && s[j] <= '9') {
                flag = 1;
                now = now * 10 + s[j] - '0';
            }
        }
        if (flag) {
            now--;
            net.insertNode(now);
            graph.getNode(now).insertNet(i);
        }
    }
}


#endif //HYPERGRAPH_PARSER_H