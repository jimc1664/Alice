#include "stdafx.h"

#include "Gem/Dis/RenderState.h"

#include "OGL_Task.h"


#include "Gem/Math/Vec2.h"

struct A { int a; };
struct B { int b; };
struct C { int c; };

typedef dpndnC_Join< dpndnC<A>, dpndnC<B> >::R dpndnC_AB;
typedef dpndnC_Join< dpndnC_AB, dpndnC<C> >::R dpndnC_ABC;
typedef dpndnC_Join< dpndnC_AB, dpndnC<A> >::R dpndnC_ABA;

void foo() {
	dpndnC_AB::Comp ab; ab.a = ab.b = 0;
	dpndnC_ABC::Comp abc; abc.c = abc.b = abc.a = 0;
	dpndnC_ABA::Comp aba; aba.a = 0;
}	

void RenderState_2d::proc( DisMain &dm ) const {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);		
	glEnable(GL_BLEND); 
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

float tRot = 0;
void RenderState_3d::proc( DisMain &dm ) const {

// GLfloat ratio;

	 int height = 768, width = 1024;
    //make sure height is always above 1
    if ( height == 0 ) {
        height = 1;
    }

	//glDisable(GL_TEXTURE_2D);		
	glDisable(GL_BLEND); 

    //calculate screen ration


  //  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    //clear the colour and depth buffer
   // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //Swith to ModelView
    glMatrixMode( GL_MODELVIEW );
    //Reset using the Indentity Matrix
    glLoadIdentity( );
		float xPos = 0, yPos = 7, zPos = -20, xLook = 0, yLook = 0, zLook = 0, xUp = 0, yUp = 1, zUp = 0;
		gluLookAt(xPos, yPos, zPos, xLook, yLook, zLook, xUp, yUp, zUp);

    glRotatef(tRot +=-0.01f,0.0f,1.0f,0.0f);

}

using namespace RS_D;

void ViewPort::proc(DisMain &dm, const Prm &d) const {
	glViewport(0, 0, d.ScrnSz.x, d.ScrnSz.y);
}
void Ortho::proc(DisMain &dm, const Prm &d) const {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float height = d.get<_Ortho>().Height;
	vec2f sz = d.get<_ScreenSize>().ScrnSz;
	sz.x *= height/sz.y; sz.y = height;
	glOrtho( 0.0, sz.x, sz.y, 0.0, 0.0, -1.0);
}
void Projection::proc(DisMain &dm, const Prm &d) const {
	vec2f sz = d.get<_ScreenSize>().ScrnSz;
	f32 ratio = sz.x / sz.y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
}