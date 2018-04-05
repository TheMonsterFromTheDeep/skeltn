#include <fstream>
#include <algorithm>
#include <regex>

#include "Dep.h"

namespace Skeltn {
	
	void processMSVCDep(const std::string& input, const std::string& output) {		
		std::ifstream in(input);
		std::ofstream out(output);
		if(!in.good()) { return; }
		if(!out.good()) { return; }
		
		std::string signal = "Note: including file: ";
		
		std::string line;
		while(getline(in, line)) {
			if(line.substr(0, signal.size()) == signal) {
				line = line.substr(signal.size());
				
				size_t cutIndex = line.find_first_not_of(' ');
				line = line.substr(cutIndex);
				
				std::replace(line.begin(), line.end(), '\\', '/');
				
				out << "\n" << line;
			}
		}
	}
	
}