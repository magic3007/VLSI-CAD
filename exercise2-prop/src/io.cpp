#include <iostream>
#include <string>
#include <algorithm>
#include "timer.hpp"

const std::string circuit_prefix_pi = "vsource:", circuit_prefix_po = "vsink:";

int Timer::_name_to_id_insert(std::string name) {
  if(name2id.count(name)) return name2id[name];
  else {
    nodes.emplace_back(name);
    return name2id[name] = n_nodes++;
  }
}

void Timer::read_delay(std::istream &is) {
  std::string s1;
  while(is >> s1) {
    if(s1 == "input") {
      std::string s2;
      is >> s2;
      nodes[_name_to_id_insert(circuit_prefix_pi + s2)].is_pi = true;
    }
    else if(s1 == "output") {
      std::string s2;
      is >> s2;
      nodes[_name_to_id_insert(circuit_prefix_po + s2)].is_po = true;
    }
    else if(s1 == "setup" || s1 == "hold") {
      std::string pin_data, pin_clk;
      timing_t value;
      is >> pin_data >> pin_clk >> value;
      int data_id = _name_to_id_insert(pin_data), clk_id = _name_to_id_insert(pin_clk);
      Constraint c = (Constraint){clk_id, data_id, value};
      if(s1 == "setup") setup_constraints.push_back(c);
      else hold_constraints.push_back(c);
    }
    else {
      std::string s2;
      float early, late;
      is >> s2 >> early >> late;
      int from_id = _name_to_id_insert(s1), to_id = _name_to_id_insert(s2);
      nodes[from_id].edges.push_back((Edge){from_id, to_id, early, late});
      nodes[to_id].rev_edges.push_back((Edge){from_id, to_id, early, late});
    }
  }
}

void Timer::read_clock_at(std::istream &is) {
  std::string s1;
  while(is >> s1) {
    if(s1 == "clock") {
      std::string s2;
      is >> s2 >> clock_period;
      clock_root = _name_to_id_insert(circuit_prefix_pi + s2);
      if(!nodes[clock_root].is_pi) {
        std::cerr << "clock root pin not in primary input" << std::endl;
      }
    }
    else if(s1 == "at") {
      std::string s2;
      float early, late;
      is >> s2 >> early >> late;
      int pin_id = _name_to_id_insert(circuit_prefix_pi + s2);
      if(!nodes[pin_id].is_pi) {
        std::cerr << "pin " << (circuit_prefix_pi + s2) <<
          " is not a primary input but was given an arrival time" << std::endl;
      }
      pi_ats.push_back((Edge){-1, pin_id, early, late});
    }
    else std::cerr << "timing input file: unknown keyword " << s1 << std::endl;
  }
}

void Timer::print_summary(std::ostream &os) {
  os << "== Simple timer report ==" << std::endl;
  os << "n_nodes = " << n_nodes << std::endl;
  os << "min setup slack = " << slack_setup << std::endl;
  os << "min hold slack = " << slack_hold << std::endl;
}
