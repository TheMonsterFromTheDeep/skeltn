#include "Project.h"

#include <fstream>

#include "Output.h"
#include "Util.h"
#include "Platform.h"

namespace Skeltn {
	enum class Section {
		Unknown,
		Project,
		Source,
		Library,
		LibraryLinux,
		LibraryOSX,
		LibraryWindows
	};
	
	void loadData(Section s, Project &proj, std::string& line) {
		if(s == Section::Unknown) return;
		
		if(s == Section::Source) {
			Util::trim(line);
			if(line.size() > 0)
				proj.files.push_back(line);
		}
		
		if(s == Section::Project) {
			auto index = line.find_last_of(':');
			if(index == std::string::npos) return;
			
			std::string propName = line.substr(0, index);
			std::string propValue = line.substr(index + 1);
			
			Util::trim(propName);
			Util::trim(propValue);
			
			if(propName == "SourceDirectory") proj.sourceDirectory = propValue;
			if(propName == "Binary") {
				proj.binaryName = propValue;
				proj.hasBinaryName = true;
			}
		}
		
		if(s == Section::Library) {
			Util::trim(line);
			if(line.size() > 0)
				proj.link.push_back(line);
		}
		
		if(s == Section::LibraryWindows) {
			Util::trim(line);
			if(line.size() > 0)
				proj.linkWindows.push_back(line);
		}
		
		if(s == Section::LibraryOSX) {
			Util::trim(line);
			if(line.size() > 0)
				proj.linkOSX.push_back(line);
		}
		
		if(s == Section::LibraryLinux) {
			Util::trim(line);
			if(line.size() > 0)
				proj.linkLinux.push_back(line);
		}
	}
	
	Project Project::load() {
		std::ifstream in(".skeltn");
		
		Project proj;
		
		if(!in.good()) {
			proj.valid = false;
			return proj;
		}
		
		proj.fileName = ".skeltn";
		
		Section currentSection = Section::Unknown;
		
		std::string line;
		while(Util::getValidLine(in, line)) {
			if(line.front() == '#') continue;
			if(line.front() == '[') {
				auto index = line.find_last_of(']');
				if(index != std::string::npos)
					line = line.substr(0, index + 1);
				
				currentSection = Section::Unknown;
				if(line == "[Project]") currentSection = Section::Project;
				if(line == "[Source]") currentSection = Section::Source;
				if(line == "[Library]") currentSection = Section::Library;
				if(line == "[Library/Linux]") currentSection = Section::LibraryLinux;
				if(line == "[Library/OSX]") currentSection = Section::LibraryOSX;
				if(line == "[Library/Windows]") currentSection = Section::LibraryWindows;
				continue;
			}
			
			loadData(currentSection, proj, line);
		}
		
		for(auto s : proj.files) {
			proj.objects.push_back("build/" + s + getPlatform().buildExtension());
		}
		
		switch(getOS()) {
			case OS::Linux:
				proj.link.insert(proj.link.end(), proj.linkLinux.begin(), proj.linkLinux.end());
				break;
			case OS::Windows:
				proj.link.insert(proj.link.end(), proj.linkWindows.begin(), proj.linkWindows.end());
				break;
			case OS::OSX:
				proj.link.insert(proj.link.end(), proj.linkOSX.begin(), proj.linkOSX.end());
				break;
		}
		
		proj.linkLinux.insert(proj.linkLinux.end(), proj.link.begin(), proj.link.end());
		proj.linkWindows.insert(proj.linkWindows.end(), proj.link.begin(), proj.link.end());
		proj.linkOSX.insert(proj.linkOSX.end(), proj.link.begin(), proj.link.end());
		
		return proj;
	}
}