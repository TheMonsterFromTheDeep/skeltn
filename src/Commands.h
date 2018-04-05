#ifndef SKELTN_COMMAND_H_
#define SKELTN_COMMAND_H_

namespace Skeltn {
	namespace Commands {
		void help(int argc, char **argv);
		void init(int argc, char **argv);
		void build(int argc, char **argv);
		void config(int argc, char **argv);
		void autoConfigMSVC(int argc, char **argv);
		void include(int argc, char **argv);
		void link(int argc, char **argv);
	}
}

#endif