/**
 * @file rctree-simplified.cpp
 * @author Zizheng Guo
 * @brief Simple RC tree implementation.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <chrono>
#include "rctree-simplified.hpp"

// extracted from OpenTimer

void Rct::read(std::istream &in, int num_nodes, int num_edges, const std::string &net_root) {
  // read nodes
  std::string node_name;
  float cap;
  for(int i = 0; i < num_nodes; ++i) {
    in >> node_name >> cap;
    auto &node = nodes[node_name];
    node.ncap = cap;
  }

  // read edges
  std::string from, to;
  float res;
  for(int i = 0; i < num_edges; ++i) {
    in >> from >> to >> res;
    auto &edge = edges.emplace_back();
    edge.fr = &nodes[from];
    edge.to = &nodes[to];
    edge.res = res;

    edge.fr->fanout.push_back(&edge);
  }

  // assign root
  root = &nodes[net_root];
}

void Rct::update_rc_timing() {
  update_load(root, nullptr);
  update_delay(root, nullptr);
  update_ldelay(root, nullptr);
  update_response(root, nullptr);
}

void Rct::dump(std::ostream &out) {
  for(const auto& [name, node] : nodes) {
    out << name << " delay: " << node.delay << " impulse: " << node.impulse << '\n';
  }
  out << std::endl;
}

int main(int argc, char **argv) {
  if(argc != 2) {
    std::cerr << "usage: ./rctree circuit.rct\n";
    std::exit(EXIT_FAILURE);
  }
  std::ifstream ifs(argv[1]);
  if(!ifs) {
    std::cerr << "failed to open " << argv[1] << '\n';
    std::exit(EXIT_FAILURE);
  }
  
  std::unordered_map<std::string, Rct> rctrees;
  std::string dummy, net_name, net_root;
  int num_nets, num_nodes, num_edges;
  
  ifs >> dummy >> dummy >> num_nets;
  int total_num_nodes = 0;
  for(int n = 0; n < num_nets; ++n) {
    ifs >> net_name >> num_nodes >> num_edges >> net_root;
    total_num_nodes += num_nodes;
    auto &rct = rctrees[net_name];
    rct.read(ifs, num_nodes, num_edges, net_root);
  }
  
  std::cout << "total_num_nodes " << total_num_nodes << std::endl << std::endl;

  auto beg = std::chrono::steady_clock::now();
  for(auto &[name, rct]: rctrees) {
    rct.update_rc_timing();
  }
  auto end = std::chrono::steady_clock::now();

  for(auto &[name, rct]: rctrees) {
    std::cout << "Net " << name << std::endl;
    rct.dump(std::cout);
  }

  std::cerr << "elapsed time to compute rc timing: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
              end-beg).count() 
            << " ms\n";
}
