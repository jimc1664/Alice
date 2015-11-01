#include "stdafx.h"
#include "Gem/String/DebugOutput.h"

void DebugOutput::Output( const char *const &c ) { OutputDebugStringA(c); }
void DebugOutput::Output( const wchar_t *const &w ) { OutputDebugStringW(w); }
