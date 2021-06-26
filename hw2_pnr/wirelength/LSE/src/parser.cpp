#include"parser.h"
#include<errno.h>
#include<cstring>

namespace Parser {
Parser::Parser(const char* fileName) {
	int pinNum = 0;

	std::string line;
	std::stringstream sline;
    std::ifstream dataFile (fileName);
	if(dataFile.is_open()) {
		getline(dataFile, line);
		sline << line;
        std::string tmp;
		bool getPinNum = false;
		bool getWeight = false;
		while(!sline.eof()) {
			sline >> tmp;
			if(!getPinNum && std::stringstream(tmp) >> pinNum) {
				getPinNum = true;
#if PARSERTEST
                std::cout << std::endl << "Parser::pinNum: " << pinNum << std::endl;
#endif
				getline(dataFile, line);
				sline.clear();
				sline << line;
				continue;
			}
			if(getPinNum && !getWeight&& std::stringstream(tmp) >> weight) {
				getWeight = true;
#if PARSERTEST
                std::cout << std::endl << "Parser::weight: " << weight << std::endl;
#endif
				getline(dataFile, line);
				sline.clear();
				sline << line;
				continue;
			}
			if(getPinNum && getWeight && std::stringstream(tmp) >> gamma) {
#if PARSERTEST
                std::cout << std::endl << "Parser::gamma: " << gamma << std::endl;
#endif
				break;
			}

		}
		vertexList.assign(pinNum, std::vector<double>(2));
		int indexi = 0;
		int tmpint = 0;
		while (getline(dataFile, line)) {
#if PARSERTEST
            std::cout << line << std::endl;
#endif
            std::stringstream dline;
			dline << line;
            std::string tmp;
			int indexj = 0;
			while (!dline.eof()){
				dline >> tmp;
				if(std::stringstream(tmp) >> vertexList[indexi][indexj]){
#if PARSERTEST
                    std::cout << vertexList[indexi][indexj] << " ";
#endif
					indexj++;
					if(indexj >= 2){
						indexi++;
						indexj = 0;
					}

				}
			}
			if(indexi == pinNum){
				break;
			}
		}
		dataFile.close();
	}
	else {
        std::cerr << "Can not open the file\n";
        std::cerr << strerror(errno) << std::endl;
		exit(1);
	}
}
};
