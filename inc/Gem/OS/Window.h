#ifndef GEM_OS_WINDOW_H
#define GEM_OS_WINDOW_H

#include "../Basic.h"
#include "../Math/Vec2.h"

namespace Gem { namespace OS {

class Window {
public:
	Window(const char * caption, const vec2u16 &size, const u32 &flags);

	enum {
		Flg_Resizeable = 1,
	};

#ifdef _WINDOWS_
	HWND hndl() { return Hndl; };
#endif

private:	
	vec2u16 Size;

	OS_Dat( HWND, ptr, Hndl );

};

}	}

#endif //GEM_OS_WINDOW_H