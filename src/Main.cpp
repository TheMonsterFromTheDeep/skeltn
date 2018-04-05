#include "Output.h"
#include "Commands.h"

int main(int argc, char **argv) {
	using namespace Skeltn;
	
	if(argc < 2) {
		Out << YellowText << "No arguments specified. Run 'skeltn help' for help.\n" << End;
		return 1;
	}
	
	std::string cmd(argv[1]);
	
	argc -= 2;
	argv += 2;
	
	if(cmd == "help") {
		Commands::help(argc, argv);
		return 0;
	}
	if(cmd == "init") {
		Commands::init(argc, argv);
		return 0;
	}
	if(cmd == "build") {
		Commands::build(argc, argv);
		return 0;
	}
	if(cmd == "config") {
		Commands::config(argc, argv);
		return 0;
	}
	if(cmd == "include") {
		Commands::include(argc, argv);
		return 0;
	}
	if(cmd == "link") {
		Commands::link(argc, argv);
		return 0;
	}
	
	Out << YellowText << "Unknown command '" << cmd << "'. Use 'skeltn help' for help.\n";
	return 1;
}