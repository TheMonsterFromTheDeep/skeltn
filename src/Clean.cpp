#include "Commands.h"

#include "FileSystem.h"
#include "Output.h"
#include "Project.h"

namespace Skeltn { namespace Commands {
		
	static void cleanPrompt(const char *path, bool (*callback)(const char*)) {
		if(!FS::getInfo(path).exists) return;
		Out << YellowText << "Remove " << BlueText << "[" << End << path << BlueText << "]: " << End;
		bool status = callback(path);
		if(status) {
			Out << GreenText << "Success\n" << End;
		}
		else {
			Out << RedText << "Failure\n" << End;
		}
	}
		
	void clean(int argc, char **argv) {
		Project p = Project::load();
		
		Out << GreenText << "Cleaning...\n";
		
		cleanPrompt("build", FS::deleteDirectory);
		if(p.hasBinaryName) {
			cleanPrompt(p.binaryName.c_str(), FS::deleteFile);
		}
	}
		
} }