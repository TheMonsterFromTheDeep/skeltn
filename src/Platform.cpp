#include "Platform.h"

namespace Skeltn {
	static Platform *platform = nullptr;
	
	static OS os;
	static bool nonDefaultOS = false;
	
	Platform &getPlatform() {
		if(platform) return *platform;
#ifdef _WIN32
		return getMSVC();
#else
		return getGCC();
#endif
	}
	
	OS getOS() {
		if(nonDefaultOS) return os;
		
#ifdef _WIN32
		return OS::Windows;
#else
	
	#ifdef __APPLE__
		return OS::OSX;
	#else
		return OS::Linux;
	#endif

#endif
	}
}