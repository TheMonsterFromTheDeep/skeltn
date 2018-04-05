#include "Dep.h"

#include <fstream>
#include <algorithm>
#include <regex>

#include "Util.h"

namespace Skeltn {
	
	void processGCCDep(const std::string& input, const std::string& output, const std::string& sourceName) {		
		std::ifstream in(input);
		std::ofstream out(output);
		if(!in.good()) { return; }
		if(!out.good()) { return; }
		
		std::string signal = sourceName + " ";
		std::string line;
		
		bool needsToCheckSignal = true;
		
		while(getline(in, line)) {
			if(needsToCheckSignal) {
				auto it = line.find(signal);
				if(it != std::string::npos)
					line = line.substr(it);
			}
			
			if(line.back() == '\\') {
				line.erase(line.back());
			}
			Util::trim(line);
			
			out << line << "\n";
		}
	}
	
}