#ifndef SKELTN_CONFIG_H_
#define SKELTN_CONFIG_H_

#include <string>
#include <vector>

namespace Skeltn {
	namespace Config {
		
		const std::string& msvcCompilerPath();
		const std::string& msvcLinkerPath();
		
		const std::string& gccCompilerPath();
		const std::string& gccLinkerPath();
		
		const std::vector<std::string>& includePaths();
		const std::vector<std::string>& linkPaths();
	}
}

#endif