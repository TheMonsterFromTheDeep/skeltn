#ifndef SKELTN_PLATFORM_H_
#define SKELTN_PLATFORM_H_

#include <string>
#include "Project.h"

namespace Skeltn {
	
	class Platform {
	public:
		virtual bool compile(const std::string& target, const std::string& source)=0;
		virtual void updateHeaders(const std::string& target, const std::string& source)=0; 
		virtual bool link(const Project& p)=0;
		virtual bool canLink(const std::string& file)=0;
		virtual std::string buildExtension()=0;
	};
	
	Platform& getMSVC();
	Platform& getGCC();
	
	Platform& getPlatform();
	
	enum class OS {
		Windows,
		OSX,
		Linux
	};
	
	OS getOS();
}

#endif