#include "PathManage.h"

#include <fstream>
#include <sstream>

#include "Output.h"
#include "FileSystem.h"
#include "Util.h"

namespace Skeltn {
	
	static void addPath(const char *type, const std::string& filePath, const std::string& toAdd) {
		std::ifstream in(filePath);
		if(!in.good()) {
			Out << RedText << "Failed to open file for reading.\n" << End;
			return;
		}
		
		bool hasEmptyLine = false;
		
		std::string line;
		while(Util::getValidLine(in, line)) {
			if(line == toAdd) {
				Out << YellowText << "Current directory already part of " << type << "!\n" << End;
				return;
			}
			if(line.size() == 0) hasEmptyLine = true;
		}
		
		in.close();
		
		std::ofstream out;
		out.open(filePath, std::ios::app);
		if(!out.good()) {
			Out << RedText << "Failed to open file for writing.\n" << End;
			return;
		}
		
		if(!hasEmptyLine) out << "\n";
		out << toAdd << "\n";
		
		Out << GreenText << "Successfully added directory to " << type << "!\n" << End;
	}
	
	static void addPathFromEnv(const char *type, const std::string& filePath, const char *env, char separator=';') {
		char *val = getenv(env);
		
		if(!val) {
			Out << RedText << "Failed to read environment variable '" << env << "'.\n" << End;
			return;
		}
		
		std::stringstream ss(val);
		std::string line;
		while(getline(ss, line, separator)) {
			addPath(type, filePath, line);
		}
		
		Out << GreenText << "Successfully imported from environment variable '" << env << "'.\n";
	}
	
	static void addEnvFromPath(const char *type, const std::string& filePath, const char *env, char separator=';') {
		char *initial = getenv(env);
		
		std::ifstream in(filePath);
		if(!in.good()) {
			Out << RedText << "Failed to open file for reading.\n" << End;
			return;
		}
		
		std::ofstream out(".skeltn-path");
		if(!out.good()) {
			Out << RedText << "Failed to open file for writing.\n" << End;
			return;
		}
		
		if(initial) out << initial;
		
		bool needsSeparator = !!initial;
		
		std::string line;
		while(Util::getValidLine(in, line)) {
			if(line.size() > 0) {
				if(needsSeparator) out << separator;
				out << line;
				needsSeparator = true;
			}
		}
		
		Out << GreenText << "Successfully exported new value to '.skeltn-path'.\n";
	}
	
	void managePaths(int argc, char **argv, const char *type) {
		if(argc < 1) {
			Out << YellowText << "Specify an action. Use 'skeltn " << type << " help' for help.\n" << End;
			return;
		}
		
		std::string folderPath = FS::getHomeDirectory();
		folderPath += "/.skeltn";
		std::string filePath = folderPath + "/" + type;
		
		FS::createDirectory(folderPath.c_str());
		
		std::string action(argv[0]);
		if(action == "add") {
			std::string toAdd = FS::getCurrentDirectory();
			
			addPath(type, filePath, toAdd);
		}
		else if(action == "remove") {
			std::string toRemove = FS::getCurrentDirectory();
			
			std::ifstream in(filePath);
			if(!in.good()) {
				Out << RedText << "Failed to open file for reading.\n" << End;
				return;
			}
			
			std::vector<std::string> lines;
			
			std::string line;
			bool hasRemove = false;
			while(Util::getValidLine(in, line)) {
				if(line == toRemove) {
					hasRemove = true;
					break;
				}
				if(line.size() > 0)
					lines.push_back(line);
			}
			
			if(!hasRemove) {
				Out << YellowText << "Current directory is not part of " << type << "!\n" << End;
				return;
			}
			
			in.close();
			
			std::ofstream out(filePath);
			if(!out.good()) {
				Out << RedText << "Failed to open file for writing.\n" << End;
				return;
			}
			
			for(auto s : lines) {
				out << s << "\n";
			}
			
			Out << GreenText << "Successfully removed directory from " << type << "!\n" << End;
		}
		else if(action == "check") {
			std::string toCheck = FS::getCurrentDirectory();
			
			std::ifstream in(filePath);
			if(!in.good()) {
				Out << RedText << "Failed to open file for reading.\n" << End;
				return;
			}
			
			std::string line;
			while(Util::getValidLine(in, line)) {
				if(line == toCheck) {
					Out << "The current directory " << GreenText << "exists" << End << " in the " << type << " list.\n";
					return;
				}
			}
			Out << "The current directory " << RedText << "does not exist" << End << " in the " << type << " list.\n";
		}
		else if(action == "list") {
			std::ifstream in(filePath);
			if(!in.good()) {
				Out << RedText << "Failed to open file for reading.\n" << End;
				return;
			}
			
			Out << GreenText << "Listing of '" << type << "':\n" << End;
			
			std::string line;
			while(Util::getValidLine(in, line)) {
				if(line.size() > 0) {
					Out << GreenText << "> " << End << line << "\n";
				}
			}
		}
		else if(action == "help") {
			Out << "$ " << BlueText << "skeltn " << type << " [action]" << End << " is used to manage file paths that skeltn searches for " << type << " files.\n\n";
			Out << YellowText << "add" << End << ": add the current directory to the list.\n";
			Out << YellowText << "remove" << End << ": remove the current directory from the list.\n";
			Out << YellowText << "check" << End << ": check if the current directory is in the list.\n";
			Out << YellowText << "list" << End << ": display all paths in the list.\n";
			std::string typeCheck(type);
			if(typeCheck == "include" || typeCheck == "link") {
				Out << YellowText << "import" << End << ": import list from relevant environment variable.\n";
				Out << YellowText << "export" << End << ": export list to file, formatted for export to a relevant environment variable.\n";
			}
			Out << YellowText << "help" << End << ": display this help.\n";
		}
		else {
			std::string typeCheck(type);
			if(typeCheck == "include") {
				if(action == "export") {
#ifdef _WIN32
					addEnvFromPath(type, filePath, "INCLUDE");
#else
					addEnvFromPath(type, filePath, "CPLUS_INCLUDE_PATH", ':');
#endif
					return;
				}
				if(action == "import") {
#ifdef _WIN32
					addPathFromEnv(type, filePath, "INCLUDE");
#else
					addPathFromEnv(type, filePath, "C_INCLUDE_PATH", ':');
					addPathFromEnv(type, filePath, "CPLUS_INCLUDE_PATH", ':');
					addPathFromEnv(type, filePath, "CPATH", ':');
#endif
					return;
				}
			}
			else if(typeCheck == "link") {
				if(action == "export") {
#ifdef _WIN32
					addEnvFromPath(type, filePath, "LIB");
#else
					addEnvFromPath(type, filePath, "LIBRARY_PATH", ':');
#endif
					return;
				}
				if(action == "import") {
#ifdef _WIN32
					addPathFromEnv(type, filePath, "LIB");
#else
					addPathFromEnv(type, filePath, "LIBRARY_PATH", ':');
#endif
					return;
				}
			}
			Out << YellowText << "Unknown action '" << action << "'. Use 'help' for help.\n" << End;
		}
	}
	
	
}