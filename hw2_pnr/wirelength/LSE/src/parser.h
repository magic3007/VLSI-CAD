#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

#define PARSERTEST 1
#define CHECKANS 0

namespace Parser {
class Parser {
private:
	std::vector<std::vector<double> > vertexList;
#if CHECKANS
	std::vector<std::vector<double> > gradientsList;
	double ans;
#endif
	double gamma;
	double weight;
public:
	Parser(const char* fileName);
	std::vector<std::vector<double> >& getVertexs() {return vertexList;};
	double getGamma() {return gamma;};
	double getWeight() {return weight;};

};
};
