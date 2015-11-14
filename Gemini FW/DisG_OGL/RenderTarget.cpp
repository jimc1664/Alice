#include "stdafx.h"

#include "Gem/Dis/Camera.h"
#include "Gem/Dis/RenderTarget.h"
#include "OGL_Task.h"

RenderTarget::RenderTarget( const vec2u16 &size ) : Size(size) {

}
void RenderTarget::render(RenderingCntx &rc) {

	/*
	for( auto &c : Cameras ) {
		c.render(dm);
	}*/
	
}
void RenderTarget::add( DisMain &dm, Dis::Camera &cam) {

	Cameras.add(&cam);
}
void RenderTarget::dropCameras( DisMain &dm ) {

	Cameras.detachAll();
}