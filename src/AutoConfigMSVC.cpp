#include "Commands.h"

#include <fstream>
#include <sstream>

#include "Output.h"
#include "FileSystem.h"

namespace Skeltn { namespace Commands {
	
	void autoConfigMSVC(int argc, char **argv) {
		std::string folderPath = FS::getHomeDirectory();
		folderPath += "/.skeltn";
		std::string includePath = folderPath + "/include";
		std::string linkPath = folderPath + "/link";
		
		FS::createDirectory(folderPath.c_str());
		
		const char *includeEnv = getenv("INCLUDE");
		if(includeEnv) {
			std::ofstream out(includePath);
			if(out.good()) {
				std::stringstream env(includeEnv);
				
				std::string line;
				while(getline(env, line, ';')) {
					out << line << "\n";
				}
				
				Out << GreenText << "Successfully configured include paths.\n" << End;
			}
			else {
				Out << RedText << "Could not open 'include' file for writing.\n";
			}
		}
		else {
			Out << YellowText << "No 'INCLUDE' environment variable found. Are you sure you are in Visual Studio Command Prompt?\n" << End;
		}
		
		const char *libEnv = getenv("LIB");
		if(libEnv) {
			std::ofstream out(linkPath);
			if(out.good()) {
				std::stringstream env(libEnv);
				
				std::string line;
				while(getline(env, line, ';')) {
					out << line << "\n";
				}
				
				Out << GreenText << "Successfully configured link paths.\n" << End;
			}
			else {
				Out << RedText << "Could not open 'link' file for writing.\n";
			}
		}
		else {
			Out << YellowText << "No 'LIB' environment variable found. Are you sure you are in Visual Studio Command Prompt?\n" << End;
		}
	}
	
} }