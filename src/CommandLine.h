#ifndef SKELTN_COMMANDLINE_H_
#define SKELTN_COMMANDLINE_H_

#include <string>
#include <vector>

namespace Skeltn {
	namespace CommandLine {
		int execute(const std::string& msg, const std::vector<std::string>& args, const char *redirectionFile);
	}
}

#endif