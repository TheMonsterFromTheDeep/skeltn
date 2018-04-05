#ifndef SKELTN_COMPILECALL_H_
#define SKELTN_COMPILECALL_H_

#include <string>
#include "Project.h"

namespace Skeltn {
	namespace Compile {
		bool compile(const std::string &target, const std::string &source);
		bool link(const Project&);
		bool testDependency(const std::string& dep);
	}
}

#endif