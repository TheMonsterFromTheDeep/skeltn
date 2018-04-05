#include "FileSystem.h"

#include "Rebuilder.h"

#include <iostream>

namespace Skeltn {
	
	Rebuilder::Rebuilder(const char *fileToBuild_) : fileToBuild(fileToBuild_), shouldRebuild(false) {
		mainInfo = FS::getInfo(fileToBuild);
		if(!mainInfo.exists) shouldRebuild = true;
	}
	
	void Rebuilder::test(const char *dependency) {
		if(shouldRebuild) return; /* Skip test if we are already rebuilding */
		
		FS::FileInfo test = FS::getInfo(dependency);
		if(test.exists && !test.invalid) {
			/* The dependency is newer */
			if(test.time > mainInfo.time) {
				shouldRebuild = true;
			}
		}
	}
	
	bool Rebuilder::build() { return shouldRebuild; }
}