#include <fstream>

#include "Output.h"
#include "FileSystem.h"

#include "Commands.h"

namespace Skeltn { namespace Commands {

	static bool createSkeltn(const char* projectName) {
		std::ofstream out(".skeltn");
		if(!out.good()) return false;
		
		std::string binName(projectName);
		auto index = binName.find_first_of(' ');
		binName = binName.substr(0, index);
		
		out <<
		"[Project]\n"
		"Name: " << projectName << "\n"
		"Version: 0\n"
		"SourceDirectory: src\n"
		"IncludeDirectory: include/" << projectName << "\n"
		"Binary: " << binName << "\n"
		"\n"
		"[Source]\n"
		"# Put your source file names here\n"
		"\n"
		"[Link]\n"
		"# Put things to link against here\n"
		"# If you have platform-specific things to link against, put them under\n"
		"# a platform-specific section (i.e. [Link/Windows], [Link/Linux])\n";
		
		return true;
	}
	
	void init(int argc, char **argv) {
		const char *projectName;
		if(argc < 1) {
			projectName = "Untitled";
			Out << YellowText << "No project name specified, using 'Untitled'\n" << End;
		}
		else {
			projectName = argv[0];
			Out << GreenText << "Initializing project '" << projectName << "'\n" << End;
		}
		
		bool hasSourceDir = true;
		bool isGood = true;
		
		if(FS::getInfo("src").exists) {
			Out << GreenText << "Source directory already exists\n" << End;
		}
		else {
			Out << "Creating source directory...";
			if(FS::createDirectory("src")) {
				Out << GreenText << "Done\n" << End;
			}
			else {
				Out << RedText << "Failed\n" << End;
				hasSourceDir = false;
				isGood = false;
			}
		}

		bool shouldInclude = true;
		bool shouldProject = true;
		if(FS::getInfo("include").exists) {
			shouldInclude = false;
			Out << GreenText << "Include directory already exists\n" << End;
			if(FS::getInfo((std::string("include/") + projectName).c_str()).exists) {
				shouldProject = false;
				Out << GreenText << "Project include directory already exists\n";
			}
		}
		if(shouldInclude) {
			Out << "Creating base include directory...";
			if(FS::createDirectory("include")) {
				Out << GreenText << "Done\n" << End;
			}
			else {
				Out << RedText << "Failed: Skipping project include directory\n" << End;
				isGood = false;
				goto noInnerInclude;
			}
		}

		if(shouldProject) {
			Out << "Creating project include directory...";
			if(FS::createDirectory((std::string("include/") + projectName).c_str())) {
				Out << GreenText << "Done\n" << End;
			}
			else {
				Out << RedText << "Failed\n" << End;
				isGood = false;
			}
		}
		
noInnerInclude:

		bool hasMainCPP = false;

		
		Out << "Creating '.skeltn'...";
		if(createSkeltn(projectName)) {
			Out << GreenText << "Done\n" << End;
		}
		else {
			Out << RedText << "Failed\n" << End;
			isGood = false;
		}
		
		if(isGood) {
			Out << GreenText << "Init succeeded!" << End << " You can now run 'skeltn build' to build the generated application.\n";
		}
		else {
			Out << YellowText << "Init failed." << End << " Certain things likely don't work correctly.";
		}
	}
	
} }
