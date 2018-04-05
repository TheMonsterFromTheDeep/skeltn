#include "Project.h"
#include "Platform.h"
#include "Config.h"
#include "CommandLine.h"
#include "FileSystem.h"
#include "Dep.h"

#include <iostream>

namespace Skeltn {
	
	static std::string compileCommand;
	static std::string linkCommand;
	static std::string ldCommand;
	
	static std::vector<std::string> compileArgs;
	static std::vector<std::string> linkArgs;
	static std::vector<std::string> ldArgs;
	
	static void prepare() {
		static bool prepared = false;
		if(prepared) return;
		
		compileCommand = Config::gccCompilerPath();
		if(compileCommand.back() != '/' && compileCommand.size() > 0) compileCommand += '/';
		compileCommand += "g++";
		
		compileArgs.push_back("-c");
		compileArgs.push_back("-MMD");
		compileArgs.push_back("-std=c++11");
		
		for(auto &s : Config::includePaths()) {
			compileArgs.push_back("-I\"" + s + '"');
		}
		
		linkCommand = Config::gccCompilerPath();
		if(linkCommand.back() != '/' && linkCommand.size() > 0) linkCommand += '/';
		linkCommand += "g++";//
		
		linkArgs.push_back("-std=c++11");
		
		for(auto &s : Config::linkPaths()) {
			linkArgs.push_back("-L\"" + s + '"');
		}
		
		ldCommand = Config::gccLinkerPath();
		if(ldCommand.back() != '/' && ldCommand.size() > 0) ldCommand += '/';	
		ldCommand += "ld";
		
		for(auto &s : Config::linkPaths()) {
			ldArgs.push_back("-L\"" + s + '"');
		}
		
		prepared = true;
	}
	
	class GCCPlatform : public Platform {
		bool compile(const std::string &target, const std::string &source) {
			prepare();
			
			std::vector<std::string> args(compileArgs.begin(), compileArgs.end());
			args.push_back(source);
			args.push_back("-o");
			args.push_back(target);
			
			if(CommandLine::execute(compileCommand, args, ".skeltn-log") != 0) return false;
			return true;
		}
		
		bool link(const Project& proj) {
			prepare();
			
			std::vector<std::string> args(linkArgs.begin(), linkArgs.end());
			
			for(auto &s : proj.objects) {
				args.push_back(s);
			}
			
			args.push_back("-o");
			args.push_back(proj.binaryName);
			
			for(auto &s : proj.link) {
				args.push_back("-l\"" + s + '"');
				std::cout << "Linking against " << s << "\n";
			}
			
			if(CommandLine::execute(linkCommand, args, ".skeltn-log") != 0) return false;
			return true;
		}
		
		void updateHeaders(const std::string &target, const std::string &source) {
			prepare();
			
			std::string tempDep = target + ".d";
			std::string actualDep = target + ".dep";
			
			processGCCDep(tempDep, actualDep, source);
			FS::deleteFile(tempDep.c_str());
		}
		
		std::string buildExtension() { return ".o"; }
		
		#define GCC_LIBRARY_NOT_FOUND 1
		
		bool canLink(const std::string &file) {
			prepare();
			
			std::vector<std::string> args(ldArgs.begin(), ldArgs.end());
			args.push_back("-l" + file);
		
			int status = CommandLine::execute(ldCommand, args, ".skeltn-log");
			
			return status == GCC_LIBRARY_NOT_FOUND;
		}
	};
	
	Platform &getGCC() {
		static GCCPlatform gp;
		return gp;
	}
}