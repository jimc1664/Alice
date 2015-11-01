#ifndef GEM_DIS_DX11_RENDERTARGET_H
#define GEM_DIS_DX11_RENDERTARGET_H

#include "Gem/Dis/RenderTarget.h"
#include "Gem/Org/sList.h"

namespace Gem {  namespace Dx11 { 

class Dx11_Main;

class RenderTarget : public Dis::RenderTarget{
public:

	void init( Dx11_Main &dm );
};

} } 

#endif //GEM_DIS_DX11_RENDERTARGET_H