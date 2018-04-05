#include "CompileCall.h"

#include <fstream>
#include <iostream>

#include "Output.h"
#include "FileSystem.h"
#include "Config.h"
#include "CommandLine.h"
#include "Platform.h"

namespace Skeltn { namespace Compile {
		
	static void printCompileLog() {
		std::ifstream in(".skeltn-log");
		if(!in.good()) return;
		Out << YellowText << "----- Compiler Log: -----\n\n" << End;
		char c;
		while(in.get(c)) {
			Out << c;
		}
		Out << YellowText << "\n----- ~~~~~~~~~~~~~ -----\n" << End;
	}
	
	bool testDependency(const std::string& dep) {
		Out << YellowText << "Verify" << BlueText << " [" << End << dep << BlueText << "]: " << End;
		
		if(!getPlatform().canLink(dep)) {
			Out << RedText << "Failure\n" << End;
			return false;
		}
		
		Out << GreenText << "Success\n" << End;
		return true;
	}
	
	bool compile(const std::string& target, const std::string& source) {
		Out << YellowText << "Compile" << BlueText << " [" << End << source << BlueText << "]: " << End;
		
		bool succeeded = true;
		
		if(!getPlatform().compile(target, source)) {
			Out << RedText << "Failure\n" << End;
			printCompileLog();
			succeeded = false;
		}
		else {
			Out << GreenText << "Success\n" << End;
		}
		
		FS::deleteFile(".skeltn-log");
		return succeeded;
	}
	
	bool link(const Project &proj) {
		Out << YellowText << "Link" << End;
		
		for(auto s : proj.objects) {
			Out << BlueText << "\n| " << End << s;
		}
		Out << BlueText << "\n> " << End;
		
		bool succeeded = true;
		
		if(!getPlatform().link(proj)) {
			Out << RedText << "Failure\n" << End;
			printCompileLog();
			succeeded = false;
		}
		else {
			Out << GreenText << "Success\n" << End;
		}
		
		FS::deleteFile(".skeltn-log");
		return succeeded;
	}
} }