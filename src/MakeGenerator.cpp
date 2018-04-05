#include "MakeGenerator.h"

#include <fstream>
#include <algorithm>

#include "Project.h"
#include "Output.h"
#include "Rebuilder.h"

namespace Skeltn {
	static void beginOutput(const char *title) {
		Out << YellowText << "Generate " << BlueText << "[" << End << title << BlueText << "]: " << End;
	}
	
	static void generateNormalMakefile(const Project& p, const char *name, const std::vector<std::string>& link) {
		std::ofstream out(name);
		beginOutput(name);
		if(!out.good()) {
			Out << RedText << "Failure\n" << End;
			return;
		}
		
		out << "CXX?=g++\n";
		out << "CXXFLAGS?=\n\n";
		out << "LINK:=";
		for(auto &s : link) {
			out << " " << s;
		}
		out << "\n\n";
		out << "SRC_DIR=" << p.sourceDirectory << "\n";
		out << "OBJ_DIR?=build\n\n";
		out << "BIN:=" << p.binaryName << "\n\n";
		out << "SRC=";
		for(auto &s : p.files) {
			out << "\\\n" << s;
		}
		out << "\n\n";
		
		out <<
		"CXXFLAGS:=$(CXXFLAGS) -std=c++11 -Wall\n\n"

		"OBJ:=$(addsuffix .o,$(SRC))\n\n"

		"SRC:=$(addprefix $(SRC_DIR)/,$(SRC))\n"
		"OBJ:=$(addprefix $(OBJ_DIR)/,$(OBJ))\n\n"

		"space:=\n"
		"space+=\n"
		"mkpath=rm -rf $(1) && mkdir -p $(1) && rm -rf $(1)\n\n"

		".PHONY: all clean\n\n"

		"all: $(BIN)\n\n"

		"$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp\n"
		"\t@$(call mkpath,$@)\n"
		"\t@printf \"Compile [$< -> $@]\\n\"\n"
		"\t@$(CXX) -MD -c $< -o $@ $(CXXFLAGS)\n\n"
			
		"$(BIN): $(OBJ)\n"
		"\t@printf \"Link\\n| $(subst $(space),\\n| ,$^)\\n> $@\\n\"\n"
		"\t@$(CXX) -o $@ $^ $(CXXFLAGS) $(addprefix -l,$(LINK))\n\n"
			
		"clean:\n"
		"\t@printf \"Clean [$(BIN)]\\nClean [$(OBJ_DIR)]\\n\"\n"
		"\t@rm -f $(BIN)\n"
		"\t@rm -rf $(OBJ_DIR)\n\n"
			
		"-include $(OBJ:.o=.d)";
		
		Out << GreenText << "Success\n" << End;
	}
	
	static void generateWindowsMakefile(const Project& p) {
		/* equivalent of mkpath: if not exist $thing ( mkdir $thing && rmdir $thing ) */
		
		std::ofstream out("make-windows");
		beginOutput("make-windows");
		if(!out.good()) {
			Out << RedText << "Failure\n" << End;
			return;
		}
		
		out << p.binaryName << ".exe: ";
		
		std::vector<std::string> objects;
		for(auto &s : p.files) {
			objects.push_back("build/" + s + ".obj");
		}
		
		for(auto &s : objects) {
			out << s << " ";
		}
		
		out << "\n";
		out << "\tlink /nologo ";
		
		for(auto &s : objects) {
			out << s << " ";
		}
		
		for(auto &s : p.linkWindows) {
			out << '"' << s << "\".lib ";
		}
		
		out << "/out:" << p.binaryName << ".exe\n\n";
		
		for(auto &s : p.files) {
			out << "build/" << s << ".obj: " << p.sourceDirectory << "/" << s << "\n";
			
			std::string fileToEnsure = "build/" + s + ".obj";
			std::replace(fileToEnsure.begin(), fileToEnsure.end(), '/', '\\');
			
			out << "\tif not exist " << fileToEnsure << " ( mkdir " << fileToEnsure << " && rmdir " << fileToEnsure << " )\n";
			out << "\tcl /nologo /c /EHsc " << "src/" << s << " /Fo" << fileToEnsure << "\n\n";
		}
		
		Out << GreenText << "Success\n" << End;
	}
	
	void generateMakefiles(const Project& p) {
		Rebuilder linux("make-linux");
		Rebuilder macos("make-macos");
		Rebuilder windows("make-windows");
		
		linux.test(p.fileName.c_str());
		macos.test(p.fileName.c_str());
		windows.test(p.fileName.c_str());
		
		if(linux.build()) {
			generateNormalMakefile(p, "make-linux", p.linkLinux);
		}
		
		if(macos.build()) {
			generateNormalMakefile(p, "make-macos", p.linkOSX);
		}
		
		if(windows.build()) {
			generateWindowsMakefile(p);
		}
	}
}