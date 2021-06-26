#include"lse.h"
#include"iostream"

namespace LSE{

Net::Net(std::vector<std::vector<double> > data, double igamma, double iweight) {
	gamma = igamma;
	weight = iweight;
	for(auto vdata: data) {
		if(vdata.size() != 2) {
            std::cerr << "LSE:: bad raw data\n";
			exit(1);
		}
		vertexList.push_back(Vertex(vdata[0],vdata[1]));
	}
}

double Net::log_sum_exp(){
    /* TODO: lse forward */
}

std::vector<std::vector<double> > Net::gradients() {
    /* TODO: lse backward */
}

};
