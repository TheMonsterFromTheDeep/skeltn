#include <fstream>
#include <algorithm>

#include "Rebuilder.h"
#include "Output.h"
#include "CompileCall.h"
#include "Project.h"
#include "FileSystem.h"
#include "Platform.h"
#include "MakeGenerator.h"

#include "Commands.h"

namespace Skeltn {
	namespace Commands {
		static bool buildFile(const Project &p, std::string name, Rebuilder &link) {
			std::string objName = "build/" + name + getPlatform().buildExtension();
			std::string srcName = p.sourceDirectory + "/" + name;
			std::string depName = objName + ".dep";
			
			Rebuilder rb(objName.c_str());
			rb.test(srcName.c_str());
			
			std::ifstream dep(depName);
			if(dep.good()) {
				std::string line;
				while(getline(dep, line)) {
					if(line.size() > 0) {
						rb.test(line.c_str());
					}
				}
			}
			
			bool status = true;
			
			if(rb.build()) {
				status = Compile::compile(objName, srcName);
			}
			
			link.test(objName.c_str());
			
			return status;
		}
		
		bool checkDependencies(Project& p) {
			std::ifstream in(".skeltn-dep");
		
			std::vector<std::string> deps;
			std::vector<std::string> old;
			
			deps.insert(deps.begin(), p.link.begin(), p.link.end());
			
			bool needsRebuild = false;
			
			if(in.good()) {
				std::string dep;
				while(getline(in, dep)) {
					if(dep.size() > 0) {
						auto index = std::find(deps.begin(), deps.end(), dep);
						if(index != deps.end())
							deps.erase(index);
						
						auto test = std::find(p.link.begin(), p.link.end(), dep);
						if(test != p.link.end())
							old.push_back(dep);
						else
							needsRebuild = true;
					}
				}
			}
			
			in.close();
			
			if(deps.size() == 0) {
				if(needsRebuild) {
					Out << GreenText << "Old dependencies detected! Removing them...\n";
					
					std::ofstream out(".skeltn-dep");
					for(auto s : old) out << s << "\n";
				}
				return true;
			}
			
			Out << GreenText << "New dependencies detected! Verifying they can be linked...\n" << End;
			
			std::vector<std::string> good;
			std::vector<std::string> bad;
			
			bool status = true;
			
			for(auto d : deps) {
				std::cerr << "Testing dependency '" << d << "'\n";
				if(Compile::testDependency(d)) {
					good.push_back(d);
				}
				else {
					bad.push_back(d);
					status = false;
				}
			}
			
			std::ofstream out(".skeltn-dep");
			
			for(auto s : old) out << s << "\n";
			for(auto s : good) out << s << "\n";
			
			if(status) {
				Out << GreenText << "All dependencies verified!\n" << End;
			}
			else {
				Out << RedText << "Could not verify the following dependencies:\n" << End;
				for(auto s : bad) {
					Out << RedText << "> " << End << s << "\n";
				}
			}
			return status;
		}
		
		void build(int argc, char **argv) {
			Project p = Project::load();
			if(!p.valid) {
				Out << RedText << "Failed to load project. Aborting build.\n" << End;
			}
			
			if(!p.hasBinaryName) {
				Out << RedText << "No binary name specified ('Binary' under [Project]). Aborting build.\n" << End;
				return;
			}
			
			if(!checkDependencies(p)) {
				Out << RedText << "Aborting build.\n" << End;
				return;
			}
			
			Out << GreenText << "Building...\n" << End;
			
			FS::createDirectory("build");
			
			Rebuilder link(p.binaryName.c_str());
			
			for(auto s : p.files) {
				if(!buildFile(p, s, link)) {
					Out << RedText << "Aborting build.\n" << End;
					return;
				}
			}
			
			if(link.build()) { if(!Compile::link(p)) return; }
			else
				Out << GreenText << "Relinking not necessary; skipping...\n" << End;
			
			generateMakefiles(p);
			
			Out << GreenText << "Build succeeded!\n" << End;
		}
	}
}