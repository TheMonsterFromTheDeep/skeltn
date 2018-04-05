#ifndef SKELTN_PROJECT_H_
#define SKELTN_PROJECT_H_

#include <vector>
#include <string>

namespace Skeltn {
	
	class Project {
	public:
		bool valid = true;
	
		std::vector<std::string> files;
		std::vector<std::string> objects;
		
		std::vector<std::string> link;
		
		std::vector<std::string> linkWindows;
		std::vector<std::string> linkOSX;
		std::vector<std::string> linkLinux;
		
		std::string fileName;
		
		std::string sourceDirectory;
		std::string binaryName;
		bool hasBinaryName = false;
		
		static Project load();
	};
}

#endif