#include "stdafx.h"

#include "RenderTarget.h"
#include "Dx10_Task.h"

RenderTarget::RenderTarget( const vec2u16 &size, OS::Window &wndw  ) : Wndw( wndw), Size(size) {

	struct Init_RT : public Dx10_Task<Init_RT> {
		Init_RT( RenderTarget *rt ) : RT(*rt) {}
		void proc( Dx10_Main &dm ) { RT.init(dm ); }
		RenderTarget &RT;
	};
	new Init_RT( this );
}

void RenderTarget::init( Dx10_Main &dm ) {

	

}

Dis::RenderTarget* Dis::RenderTarget::acquire( const vec2u16 &size, OS::Window &wndw ) {
	return new Dx10::RenderTarget( size, wndw );
}

void Dis::RenderTarget::release() {

}

