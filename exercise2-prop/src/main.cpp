#include <iostream>
#include <fstream>
#include <cstdlib>
#include "timer.hpp"

int main(int argc, char **argv) {
  std::ios::sync_with_stdio(false);
  if(argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <delay file> <timing input file>" << std::endl;
    return 1;
  }
  std::ifstream fin_delay(argv[1]), fin_timing(argv[2]);
  if(!fin_delay) {
    std::cerr << "cannot open delay file " << argv[1] << std::endl;
    return 1;
  }
  if(!fin_timing) {
    std::cerr << "cannot open timing file " << argv[2] << std::endl;
    return 1;
  }
  Timer timer;
  timer.read_delay(fin_delay);
  timer.read_clock_at(fin_timing);
  timer.compute_slack();
  timer.print_summary(std::cout);
  return 0;
}
