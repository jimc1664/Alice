#ifndef GEM_DIS_RENDERTARGET_H
#define GEM_DIS_RENDERTARGET_H

#include "DisMain.h"
#include "../OS/Window.h"
#include "../Org/dList.h"

namespace Gem { namespace Dis {

	class Camera;  class DrawList; struct RenderingCntx; //	typename DisMain;

class RenderTarget {//: public sListNode<RenderTarget> {
public:
	RenderTarget( const vec2u16 &size );

//	DrawList *volatile DrawL;

	void render(RenderingCntx &rc);
	void dropCameras(DisMain &dm);
	void add(DisMain &dm, Dis::Camera &cam);

	//void init( Dx11_Main &dm );
protected:
	dList<Camera> Cameras;

	vec2u16 Size;
};

}	}

#endif //GEM_DIS_RENDERTARGET_H