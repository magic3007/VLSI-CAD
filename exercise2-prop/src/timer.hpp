#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <optional>
#include <functional>
#include <string>

typedef double timing_t;
const timing_t INF = 1e30;

struct Edge {
  int from, to;
  timing_t early, late;
};

struct Constraint {
  int ff_clk, ff_data;
  timing_t value;
};

struct Node {
  std::string name;
  std::vector<Edge> edges;
  std::vector<Edge> rev_edges;
  bool is_pi, is_po;

  inline Node(const std::string &s): name(s), is_pi(false), is_po(false) {}
};

class Timer {
  int n_nodes = 0;
  std::unordered_map<std::string, int> name2id;
  std::vector<Node> nodes;
  std::vector<Edge> pi_ats;   // .from is not used.
  std::vector<Constraint> setup_constraints, hold_constraints;
  
  int clock_root = -1;
  timing_t clock_period;

  std::vector<int> toposeq;
  std::vector<timing_t> at_early, at_late;

  // output
  timing_t slack_setup = INF, slack_hold = INF;
  
  int _name_to_id_insert(std::string name);
  
public:
  void read_delay(std::istream &is);
  void read_clock_at(std::istream &is);
  void compute_slack();
  void print_summary(std::ostream &os);
};
