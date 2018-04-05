#ifndef SKELTN_FILESYSTEM_H_
#define SKELTN_FILESYSTEM_H_

#include <cstdint>
#include <string>

namespace Skeltn {
	namespace FS {
		class FileInfo {
		public:
			FileInfo(); /* Creates a FileInfo specifying non-existance */
			FileInfo(uint64_t time_); /* Implies the file exists */
		
			bool exists;
			uint64_t time;
			bool invalid;
			
			static FileInfo bad();
		};
		
		bool createDirectory(const char*);
		bool deleteFile(const char*);
		bool deleteDirectory(const char*);
		
		std::string getHomeDirectory();
		std::string getCurrentDirectory();
		
		FileInfo getInfo(const char*);
	}
}

#endif