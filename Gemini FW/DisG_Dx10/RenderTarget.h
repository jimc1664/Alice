#ifndef GEM_DIS_DX10_RENDERTARGET_H
#define GEM_DIS_DX10_RENDERTARGET_H

#include "Gem/Dis/RenderTarget.h"
#include "Gem/Org/sList.h"

namespace Gem {  namespace Dx10 { 

class Dx10_Main;

class RenderTarget : public Dis::RenderTarget, public sListNode<RenderTarget> {
public:
	RenderTarget( const vec2u16 &size, OS::Window &wndw );

	void init( Dx10_Main &dm );
private:
	vec2u16 Size;
	OS::Window &Wndw;
};

} }

#endif //GEM_DIS_DX10_RENDERTARGET_H