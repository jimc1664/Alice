#ifndef GEM_DIS_MAINTARGET_H
#define GEM_DIS_MAINTARGET_H

#include "DisMain.h"
#include "RenderTarget.h"
#include "DrawList.h"
//#include "../OS/Window.h"
#include "../Org/sList.h"

namespace Gem { 	
namespace OS { class Window; }

namespace Dis {

struct RenderingCntx;

class MainTarget : public RenderTarget, public sListNode<MainTarget> {
public:
	MainTarget( const vec2u16 &size, OS::Window &wndw );
	BufferedDrawList DrawL;

	void render( RenderingCntx &dm );
	void init( DisMain &dm );

protected:
	OS::Window &Wndw;
	
	Dis_Dat( HDC, ptr, Hdc  );
	Dis_Dat( HGLRC, ptr, Cntx  );

};

}	}

#endif //GEM_DIS_MAINTARGET_H