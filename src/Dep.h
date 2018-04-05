#ifndef SKELTN_MSVCDEP_H_
#define SKELTN_MSVCDEP_H_

#include <string>

namespace Skeltn {
	void processMSVCDep(const std::string& input, const std::string& output);
	void processGCCDep(const std::string& input, const std::string& output, const std::string& sourceName);
}

#endif