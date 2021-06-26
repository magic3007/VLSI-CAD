#pragma once

#include"parser.h"
#include<iostream>
#include<math.h>
#include<vector>

namespace LSE{

class Vertex {
public:
	double x;
	double y;
	Vertex(double xp, double yp) {
		x = xp;
		y = yp;
	}

};

class Net {
private:
    std::vector<Vertex> vertexList;
	double gamma;
	double weight;
public:
	Net(std::vector<std::vector<double> > data, double gamma, double weight);
	double log_sum_exp();
    std::vector<std::vector<double> > gradients();
};
};
