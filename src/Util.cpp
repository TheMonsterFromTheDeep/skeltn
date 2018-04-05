#include "Util.h"

namespace Skeltn { namespace Util {
	
	void trim(std::string& str) {
		auto first = str.find_first_not_of(' ');
		if(first != std::string::npos)
			str = str.substr(first);
		
		auto last = str.find_last_not_of(' ');
		if(last != std::string::npos)
			str = str.substr(0, last + 1);
	}

	std::string trimmed(const std::string& original) {
		std::string copy(original);
		trim(copy);
		return copy;
	}
	
	void removeFormatting(std::string &str) {
		for(;;) {
			auto it = str.find('\r');
			if(it == std::string::npos) break;
			str.erase(it);
		}
	}
	
	bool getValidLine(std::istream &stream, std::string &str) {
		for(;;) {
			if(!getline(stream, str)) return false;
			removeFormatting(str);
			if(str.size() > 0) break;
		}
		return true;
	}
} }