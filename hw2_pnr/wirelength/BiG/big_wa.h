#pragma once
#include <cassert>
#include <cmath>
#include <tuple>
#include <vector>

class BigWA {
public:
    static double forwardAndBackward(const std::vector<double> &pos, double gamma,
                                     std::vector<double> &grad);
};
