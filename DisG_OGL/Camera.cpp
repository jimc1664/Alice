#include "stdafx.h"



#include "Gem/Dis/Camera.h"
#include "Gem/Dis/RenderTarget.h"

//#include "OGL_Task.h"



void Camera::setOutput( DisMain &dm, RenderTarget &rt ) {
	rt.add(dm, *this);
}

Camera::Camera( ) {

}
void Camera::render(DisMain &dm) {

	//render();
	//DrawList 
	

}


/*
#define TaskSchedule( a, b, t, p ) \
{ \
	typedef decltype(p) tp;\
	struct a##_##b : public TaskHlpr<a##_##b,t> {\
		void proc( DisMain &dm ) { B.setOutput(dm,P); }\
		tp P;\
		b &B;\
	private: friend class TaskHlpr<a##_##b,t>;\
		a##_##b( b* _this, tp p ) : B( *_this ), P(p) {}\
	};\
	a##_##b::ctor( this, p );\
} 0 */



void Camera::setOutput( RenderTarget *rt ) {
	//TaskSchedule( setOutut, Camera, OGL_Task_Base, rt);
	TaskSchedule( DisMain, this->setOutput(cntx,*rt) );

	/*struct SetOutput_Camera : public TaskHlpr<SetOutput_Camera,OGL_Task_Base> {
		void proc( DisMain &dm ) { Cam.setOutput(dm,RT); }
		RenderTarget &RT;
		Camera &Cam;
	private: friend class TaskHlpr<SetOutput_Camera,OGL_Task_Base>;
		SetOutput_Camera( Camera* c, RenderTarget* rt ) : Cam( *c ), RT(*rt) {}
	};
	SetOutput_Camera::ctor( this, &rt ); */
}