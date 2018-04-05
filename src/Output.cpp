#include <sstream>

#include "Output.h"

static void putBeginAttribute();
static void putEndAttribute();
static void setAttribute(Skeltn::Attribute);
static void resetAttribute();

namespace Skeltn {
	Indent::Indent(const std::string &data_) : data(data_) { }
	
	Output::Output(std::ostream &stream_) : stream(stream_) { }
	
	Output& Output::operator<<(const Indent& dent) {
		blocks.push_back(dent.data);
		return *this;
	}
	
	Output& Output::unindent() {
		blocks.pop_back();
		return *this;
	}
	
	Output& Output::operator<<(Attribute a) {
		if(a == Unindent) unindent();
		else setAttribute(a);
		return *this;
	}
	
	Output& Output::reset() {
		resetAttribute();
		return *this;
	}
	
	void Output::pre() {
		for(auto &s : blocks) { stream << s; }
		putBeginAttribute();
	}
	
	void Output::post() {
		putEndAttribute();
	}
	
	Output& Output::operator<<(const std::string &str) {
		pre();
		size_t extra = 0;
		for(auto &s : blocks) { extra += s.size(); }
		size_t size = 0;
		
		std::istringstream ss(str);
		std::string next;
		
		bool needsSpace = false;
		while(getline(ss, next, ' ')) {
			if(needsSpace) stream << ' ';
			needsSpace = true;
			stream << next;
			size += next.size() + 1;
			if(extra + size > 80) {
				stream << "\n";
				needsSpace = false;
				size = 0;
				extra = 0;
			}
			if(next.find(' ') != std::string::npos) {
				size = 0;
				extra = 0;
				needsSpace = false;
			}
		}
		if(str[str.size() - 1] == ' ') stream << ' ';
		post();
		return *this;
	}
	
	Output& Output::operator<<(const char *str) {
		return operator<<(std::string(str));
	}
	
	Output Out(std::cout);
}

#ifdef _WIN32

#include <Windows.h>

static HANDLE hConsole;
static CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
static WORD savedAttributes;

static bool enabled = false;

static WORD fgColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
static WORD bgColor;

void initConsole() {
	static bool hasInitialized = false;
	if(hasInitialized) return;
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hConsole == INVALID_HANDLE_VALUE) { goto done; }
	
	if(!GetConsoleScreenBufferInfo(hConsole, &consoleInfo)) { goto done; }
	
	enabled = true;
	
	savedAttributes = consoleInfo.wAttributes;
	
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	
done:
	hasInitialized = true;
}

void putBeginAttribute() {
	initConsole();
	
	if(enabled) {
		SetConsoleTextAttribute(hConsole, fgColor | bgColor);
	}
}

void putEndAttribute() {
	initConsole();
	
	if(enabled) {
		SetConsoleTextAttribute(hConsole, savedAttributes);
	}
}

void resetAttribute() {
	fgColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	bgColor = 0;
}

void setAttribute(Skeltn::Attribute a) {
	using namespace Skeltn;
	switch(a) {
		case WhiteText: fgColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
		case BlackText: fgColor = 0; break;
		case RedText: fgColor = FOREGROUND_RED; break;
		case BlueText: fgColor = FOREGROUND_BLUE; break;
		case GreenText: fgColor = FOREGROUND_GREEN; break;
		case CyanText: fgColor = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
		case PurpleText: fgColor = FOREGROUND_RED | FOREGROUND_BLUE; break;
		case YellowText: fgColor = FOREGROUND_RED | FOREGROUND_GREEN; break;
		case WhiteBG: bgColor = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE; break;
		case BlackBG: bgColor = 0; break;
		case RedBG: bgColor = BACKGROUND_RED; break;
		case BlueBG: bgColor = BACKGROUND_BLUE; break;
		case GreenBG: bgColor = BACKGROUND_GREEN; break;
		case CyanBG: bgColor = BACKGROUND_BLUE | BACKGROUND_GREEN; break;
		case PurpleBG: bgColor = BACKGROUND_RED | BACKGROUND_BLUE; break;
		case YellowBG: bgColor = BACKGROUND_RED | BACKGROUND_GREEN; break;
		case End:
			resetAttribute();
			break;
		default: { }
	}
}

#else
	
const char *fgCode = "37";
const char *bgCode = "40";

void putBeginAttribute() {
	std::cout << "\e[" << fgCode << ";" << bgCode << "m";
}

void putEndAttribute() {
	std::cout << "\e[0m";
}

void setAttribute(Skeltn::Attribute a) {
	using namespace Skeltn;
	switch(a) {
		case WhiteText: fgCode = "37"; break;
		case BlackText: fgCode = "30"; break;
		case RedText: fgCode = "31"; break;
		case GreenText: fgCode = "32"; break;
		case YellowText: fgCode = "33"; break;
		case BlueText: fgCode = "34"; break;
		case PurpleText: fgCode = "35"; break;
		case CyanText: fgCode = "36"; break;
		case WhiteBG: bgCode = "47"; break;
		case BlackBG: bgCode = "40"; break;
		case RedBG: bgCode = "41"; break;
		case GreenBG: bgCode = "42"; break;
		case YellowBG: bgCode = "43"; break;
		case BlueBG: bgCode = "44"; break;
		case PurpleBG: bgCode = "45"; break;
		case CyanBG: bgCode = "46"; break;
		case End:
			resetAttribute();
			break;
		default: { }
	}
}

void resetAttribute() {
	fgCode = "37";
	bgCode = "40";
}

#endif