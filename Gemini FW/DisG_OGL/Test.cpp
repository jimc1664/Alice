#include "stdafx.h"

#include "Gem/Math/vec3.h"

#include "Gem/Dis/Camera.h"
#include "Gem/Dis/RenderTarget.h"
#include "Gem/Dis/DrawList.h"
#include "Gem/Dis/Texture.h"
#include "Gem/Dis/Mesh.h"
#include "Gem/Dis/ShaderProg.h"

#include "OGL_Task.h"

#include "Gem/ConCur/ConCur.h"


#include "Gem/Math/vec4.h"
#include "Gem/Math/matrix4.h"
#include <Gem/Math/Randomizer.h>
#include "Gem/Org/Ary.h"

#include "Gem/Scene3/TestObj.h"
#include "Gem/Scene3/Texture.h"
#include "Gem/Scene3/Passive.h"
#include "Gem/Scene3/Mesh.h"


#include "../procedural/simplexnoise.h"

#include <iostream>
#include <fstream>

bool debugFlag(const u32 &i);

void DrawTestCube::proc(RenderingCntx &rc) { 

	if( debugFlag(3) ) {
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if( debugFlag(4) ) return;

	auto projMatrix = mat4f::projection(45.0f * DEG_TO_RAD, 1024.0f/768.0f, 0.001f, 100.0f);  //perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	auto mvp = Trans.as<mat4f>()*projMatrix;// *worldMatrix;
	projMatrix = mat4f::identity();
	Prog.apply(rc, projMatrix);


	glUseProgram(0);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION_MATRIX);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glLoadIdentity();

	Tex.Hdwr->apply(rc);

	//vec3f camPos = vec3f(0, 0, 0)* Trans.inverse();


	f32 scl = 1.0f, h = 768.0f /1024.0f, w = 1.0f / h;
	glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
	glTexCoord2f(0, -2.0f);
	glVertex3f(-1.0f, 3.0f, 0.5f);              // Top
	glTexCoord2f(2, 0.0f);
	glVertex3f(3.0f, -1.0f, 0.5f);              // Bottom Right
	glTexCoord2f(0, 0.0f);
	glVertex3f( -1.0f, -1.0f * scl, 0.5f);              // Bottom Left
//	glVertex3f(0.0f  * scl, 1.0f * scl, 0.5f);              // Top


	glEnd();

	glMatrixMode(GL_PROJECTION_MATRIX);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPopMatrix();

	glBindVertexArray(0);

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable(GL_CULL_FACE); 
}
