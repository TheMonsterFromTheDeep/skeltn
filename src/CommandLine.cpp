#include "CommandLine.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include <cstring>
#include "Output.h"

#include <fstream>

namespace Skeltn { namespace CommandLine {
	
	int execute(const std::string &cmd, const std::vector<std::string>& args, const char *redirectionFile) {
#ifdef _WIN32
		std::string wholeCommand = cmd;
		for(auto &s : args) {
			wholeCommand += " " + s;
		}

		char *commandline = new char[wholeCommand.length() + 1];
		strcpy(commandline, wholeCommand.c_str());
		
		STARTUPINFO si={sizeof(si)};
		PROCESS_INFORMATION pi;
		
		HANDLE redir = INVALID_HANDLE_VALUE;
		
		SECURITY_ATTRIBUTES redirSA={sizeof(redirSA)};
			
		redirSA.bInheritHandle = TRUE;
		
		DWORD exitCode = 0;
		
		if(redirectionFile != NULL) {
			
			
			redir = CreateFile(redirectionFile,
				GENERIC_WRITE,
				0,
				&redirSA,
				CREATE_ALWAYS,
				0,
				NULL);
			if(redir != INVALID_HANDLE_VALUE) {
				si.hStdOutput = redir;
				si.hStdError = redir;
				si.dwFlags |= STARTF_USESTDHANDLES;
			}
		}
		
		if(CreateProcess(NULL,
			commandline,
			NULL,
			NULL,
			TRUE,
			0,
			NULL,
			NULL,
			&si,
			&pi)
		) {
			WaitForSingleObject(pi.hProcess, INFINITE);
			
			GetExitCodeProcess(pi.hProcess, &exitCode);
			
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		
		if(redir != INVALID_HANDLE_VALUE)
			CloseHandle(redir);
		
		delete[] commandline;
		return exitCode;
#else
		pid_t pid = fork();
		pid_t ret;
		int status;
		if(pid == -1) {
			return 0; /* TODO: Return separate value for "failed to invoke" */
		}
		
		if(pid != 0) {
			while((ret = waitpid(pid, &status, 0)) == -1) {
				if(errno != EINTR) break;
			}
			
			if(WIFEXITED(status)) {
				int es = WEXITSTATUS(status);
				return es;
			}
			else { return 0; /* See above TODO */ }
		}
		else {
			int fd = open(redirectionFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			
			dup2(fd, 1);
			dup2(fd, 2);
			
			close(fd);
			
			char **argv = new char*[args.size() + 2];
			argv[0] = new char[cmd.size() + 1];
			strcpy(argv[0], cmd.c_str());
			for(size_t i = 0; i < args.size(); ++i) {
				argv[i + 1] = new char[args[i].size() + 1];
				strcpy(argv[i + 1], args[i].c_str());
			}
			argv[args.size() + 1] = NULL;
			
			char *command = new char[cmd.size() + 1];
			strcpy(command, cmd.c_str());
			
			execve(command, argv, environ);
			exit(127);
		}
#endif
	}
	
} }