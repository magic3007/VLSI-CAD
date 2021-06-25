/**
 * @file rctree-simplified.hpp
 * @author Zizheng Guo
 * @brief Simple RC tree declarations.
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <list>

// extracted from OpenTimer, by Tsung-Wei Huang.
// eliminated EL/RF corners and helper functions.

struct Rct;
struct RctNode;
struct RctEdge;

struct RctEdge {
  RctNode *fr, *to;
  float res;
};

struct RctNode {
  // input
  float ncap;
  std::list<RctEdge *> fanout;

  // output (exercise)
  float load, delay, ldelay, beta, impulse;

  inline RctNode(): ncap(0.), load(0.), delay(0.), ldelay(0.), beta(0.), impulse(0.) {}
};

struct Rct {
  std::unordered_map<std::string, RctNode> nodes;
  std::list<RctEdge> edges;
  RctNode *root;

  void read(std::istream &in, int num_nodes, int num_edges, const std::string &net_root);
  void update_rc_timing();
  void dump(std::ostream &out);

  // Exercise: you shall implement the procedures below
  void update_load(RctNode *u, RctNode *parent);
  void update_delay(RctNode *u, RctNode *parent);
  void update_ldelay(RctNode *u, RctNode *parent);
  void update_response(RctNode *u, RctNode *parent);
};
