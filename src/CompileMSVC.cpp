#include "Project.h"
#include "Platform.h"
#include "Config.h"
#include "Dep.h"
#include "CommandLine.h"
#include "FileSystem.h"

#include <iostream>

namespace Skeltn {
	
	static std::string compileCommand;
	static std::string linkCommand;
	
	static std::vector<std::string> compileArgs;
	static std::vector<std::string> preprocessArgs;
	static std::vector<std::string> linkArgs;
	
	static void prepare() {
		static bool prepared = false;
		if(prepared) return;
		
		compileCommand = Config::msvcCompilerPath();
		if(compileCommand.back() != '/' && compileCommand.size() > 0) compileCommand += '/';
		compileCommand = '"' + compileCommand + "cl\"";
		
		compileArgs.push_back("/nologo");
		
		for(auto s : Config::includePaths()) {
			compileArgs.push_back("/I\"" + s + '"');
		}
		
		preprocessArgs.insert(preprocessArgs.begin(), compileArgs.begin(), compileArgs.end());
		preprocessArgs.push_back("/P");
		preprocessArgs.push_back("/showIncludes");
		
		
		compileArgs.push_back("/c");
		compileArgs.push_back("/EHsc");
		
		linkCommand = Config::msvcLinkerPath();
		if(linkCommand.back() != '/' && linkCommand.size() > 0) linkCommand += '/';
		linkCommand = '"' + linkCommand + "link\"";
		
		linkArgs.push_back("/nologo");
		
		for(auto s : Config::linkPaths()) {
			linkArgs.push_back("/LIBPATH:\"" + s + '"');
		}
		
		prepared = true;
	}
	
	class MSVCPlatform : public Platform {
		bool compile(const std::string &target, const std::string &source) {
			prepare();
			
			std::vector<std::string> args(compileArgs.begin(), compileArgs.end());
			args.push_back(source);
			args.push_back("/Fo\"" + target + '"');
			
			if(CommandLine::execute(compileCommand, args, ".skeltn-log") != 0) return false;
			return true;
		}
		
		bool link(const Project& proj) {
			prepare();
			
			std::vector<std::string> args(linkArgs.begin(), linkArgs.end());
			for(auto &s : proj.objects) {
				args.push_back(s);
			}
			
			args.push_back("/out:" + proj.binaryName + ".exe");
			
			for(auto &s : proj.link) {
				args.push_back('"' + s + ".lib\"");
			}
			
			if(CommandLine::execute(linkCommand, args, ".skeltn-log") != 0) return false;
			return true;
		}
		
		void updateHeaders(const std::string &target, const std::string &source) {
			prepare();
			
			std::string tempDep = target + ".dep~";
			std::string actualDep = target + ".dep";
			
			std::vector<std::string> args(preprocessArgs.begin(), preprocessArgs.end());
			args.push_back('"' + source + '"');
			CommandLine::execute(compileCommand, args, tempDep.c_str());
			
			std::string fileToRemove = source;
			auto index = fileToRemove.find_last_of('.');
			if(index != std::string::npos)
				fileToRemove = fileToRemove.substr(0, index);
			fileToRemove += ".i";
			index = fileToRemove.find_last_of('/');
			if(index != std::string::npos)
				fileToRemove = fileToRemove.substr(index + 1);
			FS::deleteFile(fileToRemove.c_str());
			
			processMSVCDep(tempDep, actualDep);
			FS::deleteFile(tempDep.c_str());
		}
		
		std::string buildExtension() { return ".obj"; }
		
		#define MSVC_LIBRARY_NOT_FOUND 1181
		
		bool canLink(const std::string &file) {
			prepare();
			
			std::vector<std::string> args(linkArgs.begin(), linkArgs.end());
			args.push_back('"' + file + ".lib\"");
		
			int status = CommandLine::execute(linkCommand, args, ".skeltn-log");
			
			return status != MSVC_LIBRARY_NOT_FOUND;
		}
	};
	
	Platform &getMSVC() {
		static MSVCPlatform mp;
		return mp;
	}
}