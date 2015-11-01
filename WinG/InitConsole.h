#ifndef GEM_WING_INITCONSOLE_H
#define GEM_WING_INITCONSOLE_H

#include <iostream>
#include <io.h>
#include <fcntl.h>

namespace Gem {		namespace WinG {

void initConsole() {

	AllocConsole();	

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	s32 hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;

};

} }

#endif //GEM_WING_INITCONSOLE_H