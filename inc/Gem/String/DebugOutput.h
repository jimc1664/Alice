#ifndef GEM_STRING_DEBUGOUTPUT_H
#define GEM_STRING_DEBUGOUTPUT_H

#include "StrStream.h"

namespace Gem {

class DebugOutput : public StrStream<DebugOutput>, Uncopyable {
public:
	DebugOutput() {}
/*#if defined(_WINDOWS_)
#define Ouptut _Ouptut
#endif*/
	DebugOutput& operator<<( const CStr &c )		{ Output(c); return *this; } 
	DebugOutput& operator<<( const CStrW &w )	{ Output(w); return *this; }
private:
/*#if defined(_WINDOWS_)
#undef Ouptut
	void _Output( const char *const &c )		{ OutputDebugStringA(c); }
	void _Output( const wchar_t *const &w )	{ OutputDebugStringW(w); }
#endif */
	void Output( const char *const &c );
	void Output( const wchar_t *const &w );
};

}

#endif //GEM_STRING_DEBUGOUTPUT_H