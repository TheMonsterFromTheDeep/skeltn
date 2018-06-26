#include <string>
#include <limits>

/* TODO: Use Win32 api for Windows filesystem stuff */
#ifdef _WIN32
	#define mkdir(path, mode) _mkdir(path)
	
	#include <Windows.h>
	#include <Shlobj.h>
#else
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <pwd.h>
	#include <limits.h>
	#include <ftw.h>
#endif

#include "Output.h"

#include "FileSystem.h"

namespace Skeltn { namespace FS {
	FileInfo::FileInfo() : exists(false), time(0), invalid(false) { }
	
	FileInfo::FileInfo(uint64_t time_) : exists(true), time(time_), invalid(false) { }
	
	FileInfo FileInfo::bad() {
		FileInfo f;
		f.invalid = true;
		return f;
	}
	
	bool createDirectory(const char *path) {
#ifdef _WIN32
		return CreateDirectory(path, NULL);
#else
		#define FILE_PERMS 0755 /* this ***MUST*** be octal */
		return !mkdir(path, FILE_PERMS);
#endif
	}
	
	bool deleteFile(const char *path) {
#ifdef _WIN32
		return DeleteFile(path);
#else
		return !remove(path);
#endif
	}
	
	FileInfo getInfo(const char *path) {
#ifdef _WIN32
		DWORD attr = GetFileAttributes(path);
		/* file does not exist */
		if(attr == INVALID_FILE_ATTRIBUTES) return FileInfo();

		HANDLE f = CreateFile(
			path,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		
		if(f == INVALID_HANDLE_VALUE) {
			return FileInfo::bad();
		}
		
		FILETIME lastWriteTime;
		if(!GetFileTime(f, NULL, NULL, &lastWriteTime)) {
			CloseHandle(f);
			return FileInfo::bad();
		}
		
		CloseHandle(f);
		return FileInfo(lastWriteTime.dwLowDateTime | ((uint64_t)lastWriteTime.dwHighDateTime << 32));
#else
		if(access(path, F_OK) == -1) return FileInfo(); /* file does not exist */

		struct stat attr;
		if(stat(path, &attr)) return FileInfo::bad();
		
		return FileInfo(attr.st_mtime);
#endif
	}
	
	std::string getHomeDirectory() {
#ifdef _WIN32
		WCHAR path[MAX_PATH];
		if(SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
			std::wstring ws(path);
			std::string str(ws.begin(), ws.end());
			return str;
		}
		else {
			const char *homedrive = getenv("HOMEPATH");
			const char *homedir = getenv("HOMEDRIVE");
			if(homedrive && homedir) {
				return std::string(homedrive) + std::string(homedir);
			}
			
			goto noHomeDir;
		}
#else
		const char *homedir = NULL;
		if((homedir = getenv("XDG_CONFIG_HOME")) == NULL) {
			if((homedir = getenv("HOME")) == NULL) {
				homedir = getpwuid(getuid())->pw_dir;
			}
		}
		if(!homedir) goto noHomeDir;
		return std::string(homedir);
#endif

noHomeDir:
		Out << "\n" << RedText << "An attempt to retrieve the home directory failed. Quitting skeltn...\n" << End;
		exit(-20);
	}
	
	std::string getCurrentDirectory() {
#ifdef _WIN32
		DWORD bufSize = GetCurrentDirectory(0, NULL);
		char *buffer = new char[bufSize];
		GetCurrentDirectory(bufSize, buffer);
		
		std::string dir(buffer);
		delete[] buffer;
		
		return dir;
#else
		char buf[PATH_MAX];
		if(getcwd(buf, PATH_MAX) != NULL) {
			return std::string(buf);
		}
		
		Out << "\n" << RedText << "An attempt to retrieve the working directory failed. Quitting skeltn...\n" << End;
		exit(-40);
#endif
	}
	
#ifndef _WIN32
	int deleteInnerFile(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
		return remove(fpath);
	}
	
	bool deleteDirectory(const char *path) {
		return !nftw(path, deleteInnerFile, 64, FTW_DEPTH | FTW_PHYS);
	}
#else
	bool deleteDirectory(const char *path) {
		SHFILEOPSTRUCT fileOp = {
			NULL,
			FO_DELETE,
			path,
			"",
			FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT,
			FALSE,
			0,
			""
		};
    	return !SHFileOperation(&fileOp);
	}
#endif
	
} }
