#include "Config.h"

#include <fstream>

#include "FileSystem.h"
#include "Commands.h"
#include "Output.h"

namespace Skeltn { namespace Config {
	
	static std::string msvcCompilerPathVal = "";
	static std::string msvcLinkerPathVal = "";
	
	static std::string gccCompilerPathVal = "";
	static std::string gccLinkerPathVal = "";
	
	static std::vector<std::string> includePathsVal;
	static std::vector<std::string> linkPathsVal;
	
	static void loadProp(const std::string& propName, const std::string& propValue) {
		if(propName == "msvc-compiler-path") { msvcCompilerPathVal = propValue; }
		if(propName == "msvc-linker-path") { msvcLinkerPathVal = propValue; }
		if(propName == "gcc-compiler-path") { gccCompilerPathVal = propValue; }
		if(propName == "gcc-linker-path") { gccLinkerPathVal = propValue; }
	}
	
	static void loadConfig() {
		static bool isLoaded = false;
		if(isLoaded) return;
		
		std::string configPath = FS::getHomeDirectory();
		configPath += "/.skeltn/config";
		
		std::ifstream input(configPath);
		if(!input.good()) { isLoaded = true; return; }
		
		std::string line;
		while(getline(input, line)) {
			auto pos = line.find(':');
			if(pos != std::string::npos) {
				std::string propName = line.substr(0, pos);
				std::string propValue = line.substr(pos + 1);
				loadProp(propName, propValue);
			}
		}
		
		isLoaded = true;
	}
	
	const std::string& msvcCompilerPath() {
		loadConfig();
		return msvcCompilerPathVal;
	}
	
	const std::string& msvcLinkerPath() {
		loadConfig();
		return msvcLinkerPathVal;
	}
	
	const std::string& gccCompilerPath() {
		loadConfig();
		return gccCompilerPathVal;
	}
	
	const std::string& gccLinkerPath() {
		loadConfig();
		return gccLinkerPathVal;
	}
	
	const std::vector<std::string>& includePaths() {
		bool isLoaded = false;
		if(isLoaded) return includePathsVal;
		
		std::string includePath = FS::getHomeDirectory();
		includePath += "/.skeltn/include";
		
		std::ifstream in(includePath);
		if(in.good()) {
			std::string line;
			while(getline(in, line)) {
				if(line.size() > 0) {
					includePathsVal.push_back(line);
				}
			}
		}
		
		isLoaded = true;
		return includePathsVal;
	}
	
	const std::vector<std::string>& linkPaths() {
		bool isLoaded = false;
		if(isLoaded) return linkPathsVal;
		
		std::string linkPath = FS::getHomeDirectory();
		linkPath += "/.skeltn/link";
		
		std::ifstream in(linkPath);
		if(in.good()) {
			std::string line;
			while(getline(in, line)) {
				if(line.size() > 0) {
					linkPathsVal.push_back(line);
				}
			}
		}
		
		isLoaded = true;
		return linkPathsVal;
	}
} 

namespace Commands {

	void config(int argc, char **argv) {
		std::string folderPath = FS::getHomeDirectory();
		folderPath += "/.skeltn";
		std::string configPath = folderPath + "/config";
		
		FS::createDirectory(folderPath.c_str());
		
		std::ofstream out(configPath);
		
		if(!out.good()) {
			Out << RedText << "Could not open config file for writing.\n" << End;
			return;
		}
		
		std::string path = FS::getCurrentDirectory();
		
#ifdef _WIN32
		out << "msvc-compiler-path:" << path << "\nmsvc-linker-path:" << path << "\n";
#else
		out << "gcc-compiler-path:" << path << "\ngcc-linker-path:" << path << "\n";
#endif
		
		Out << GreenText << "Successfully wrote config file.\n" << End;
	}
	
} }