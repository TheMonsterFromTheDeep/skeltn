#include "Commands.h"

#include "PathManage.h"

namespace Skeltn { namespace Commands {
	
	void include(int argc, char **argv) {
		managePaths(argc, argv, "include");
	}
	
	void link(int argc, char **argv) {
		managePaths(argc, argv, "link");
	}
	
} }