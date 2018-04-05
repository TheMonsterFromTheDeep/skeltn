#ifndef SKELTN_UTIL_H_
#define SKELTN_UTIL_H_

#include <string>
#include <istream>

namespace Skeltn { namespace Util {
	void trim(std::string& str);
	std::string trimmed(const std::string& original);
	
	void removeFormatting(std::string &str);
	bool getValidLine(std::istream &stream, std::string &str);
} }

#endif