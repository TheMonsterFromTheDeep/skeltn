#ifndef SKELTN_OUTPUT_H_
#define SKELTN_OUTPUT_H_

#include <iostream>
#include <vector>
#include <string>

namespace Skeltn {
	class Indent {
	public:
		std::string data;
		Indent(const std::string&);
	};
	
	enum Attribute {
		BlackText,
		WhiteText,
		RedText,
		GreenText,
		BlueText,
		CyanText,
		PurpleText,
		YellowText,
		BlackBG,
		WhiteBG,
		RedBG,
		GreenBG,
		BlueBG,
		CyanBG,
		PurpleBG,
		YellowBG,
		End,
		Unindent
	};
	
	class Output {
		std::vector<std::string> blocks;
		std::ostream &stream;
		
		void pre();
		void post();
	public:	
		Output(std::ostream&);
		Output& operator<<(const Indent&);
		Output& unindent();
		Output& operator<<(Attribute);
		Output& reset();
		
		Output& operator<<(const char*);
		Output& operator<<(const std::string&);
		
		template<class T>
		Output& operator<<(T what) {
			pre();
			stream << what;
			post();
			return *this;
		}
	};
	
	extern Output Out;
}

#endif