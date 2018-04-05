#include <fstream>

#include "Output.h"
#include "FileSystem.h"

#include "Commands.h"

namespace Skeltn { namespace Commands {
	static bool createMainCPP() {
		std::ofstream out("src/Main.cpp");
		if(!out.good()) return false;
		
		out <<
		"#include <iostream>\n\n"
		"int main() {\n"
		"\tstd::cout << \"Hello, world!\\n\";\n"
		"}\n";
		
		return true;
	}
	
	static bool createSkeltn(const char* projectName, bool hasMainCPP) {
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
		"IncludeDirectory: include/$Name\n"
		"Binary: " << binName << "\n"
		"\n"
		"[Source]\n";
		if(hasMainCPP) out << "Main.cpp\n";
		out <<
		"# Put your source file names here\n"
		"\n"
		"[Library]\n"
		"# Put things to link against here\n"
		"# If you have platform-specific things to link against, put them under\n"
		"# a platform-specific section (i.e. [Library/Windows], [Library/Linux])\n"
		"\n"
		"# If everything went well you can now run 'skeltn build' and a hello world\n"
		"# application will build successfully!\n";
		
		return true;
	}
	
	void init(int argc, char **argv) {
		const char *projectName;
		if(argc < 1) {
			projectName = "Untitled Project";
			Out << YellowText << "No project name specified, using 'Untitled Project'\n" << End;
		}
		else {
			projectName = argv[0];
			Out << GreenText << "Initializing project '" << projectName << "'\n" << End;
		}
		
		bool hasSourceDir = true;
		bool isGood = true;
		
		Out << "Creating source directory...";
		if(FS::createDirectory("src")) {
			Out << GreenText << "Done\n" << End;
		}
		else {
			Out << RedText << "Failed\n" << End;
			hasSourceDir = false;
			isGood = false;
		}
		
		Out << "Creating base include directory...";
		if(FS::createDirectory("include")) {
			Out << GreenText << "Done\n" << End;
		}
		else {
			Out << RedText << "Failed: Skipping project include directory\n" << End;
			isGood = false;
			goto noInnerInclude;
		}
		
		Out << "Creating project include directory...";
		if(FS::createDirectory((std::string("include/") + projectName).c_str())) {
			Out << GreenText << "Done\n" << End;
		}
		else {
			Out << RedText << "Failed\n" << End;
			isGood = false;
		}
		
noInnerInclude:

		bool hasMainCPP = false;

		if(hasSourceDir) {
			Out << "Creating simple 'Main.cpp'...";
			if(createMainCPP()) {
				Out << GreenText << "Done\n" << End;
				hasMainCPP = true;
			}
			else {
				Out << RedText << "Failed\n" << End;
				isGood = false;
			}
		}
		else {
			Out << YellowText << "Skipping 'Main.cpp' because 'src' could not be created\n" << End;
		}
		
		Out << "Creating '.skeltn'...";
		if(createSkeltn(projectName, hasMainCPP)) {
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