#ifndef SKELTN_REBUILDER_H_
#define SKELTN_REBUILDER_H_

#include <cstdint>

#include "FileSystem.h"

namespace Skeltn {
	
	class Rebuilder {
		FS::FileInfo mainInfo;
		const char *fileToBuild;
		
		bool shouldRebuild;
	public:
		Rebuilder(const char* fileToBuild_);
		
		void test(const char *dependency);
		
		bool build();
	};
	
}

#endif