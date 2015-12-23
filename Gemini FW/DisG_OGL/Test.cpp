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
//#include <Gem/Math/Randomizer.h>
#include "Gem/Org/Ary.h"

#include "Gem/Scene3/Camera.h"
#include "Gem/Scene3/TestObj.h"
#include "Gem/Scene3/Texture.h"
//#include "Gem/Scene3/Passive.h"
//#include "Gem/Scene3/Mesh.h"


//#include "../procedural/simplexnoise.h"

#include <iostream>
#include <fstream>

bool debugFlag(const u32 &i);

void DrawTestCube::proc(RenderingCntx &rc) { 
	return;
	if( debugFlag(3) ) {
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if( debugFlag(4) ) return;

	auto projMatrix = mat4f::projection(45.0f * DEG_TO_RAD, 1024.0f/768.0f, 0.001f, 100.0f);  //perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	auto mvp = Trans.as<mat4f>()*projMatrix;// *worldMatrix;
	projMatrix = mat4f::identity();
	Prog.apply(rc, projMatrix, Trans.as<mat4f>());


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

void checkErr() {

		int err = glGetError();
		auto es = glewGetErrorString( err );
		if( err ) {
			int a = 0;
		}
}

vec2s16 sSz(1024, 768 );
class DefferedStuff {
public:


	DefferedStuff() : Post ( Dis::ShaderProg::fromFile( CSTR("Media//shaders//defferedVS.glsl"), CSTR("Media//shaders//deffered_PostFS.glsl")) ), 
		LightPass ( Dis::ShaderProg::fromFile( CSTR("Media//shaders//defferedVS.glsl"), CSTR("Media//shaders//deffered_LightPassFS.glsl")) ) {
		SetLastError(0);
		glGenTextures( TexCnt, Textures);

		glGenTextures(1, &DepthTex);

		glBindTexture(GL_TEXTURE_2D, DepthTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, sSz.x, sSz.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT,  NULL);

		GLuint fbos[2];
		
		glGenFramebuffers(2, fbos); 
		for( u32 i = 2; i--; ) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbos[i] );
			if(!i) //norm only?
				glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTex, 0);

		///	for( u32 i = TexCnt; i--; ) {
			glBindTexture(GL_TEXTURE_2D, Textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, sSz.x, sSz.y, 0, GL_RGBA, GL_FLOAT, NULL);
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, Textures[i], 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);  //todo - use shader to fix level 0 
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//	}


			GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 }; 
			glDrawBuffers( 1, DrawBuffers);

			GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);

			if (err != GL_FRAMEBUFFER_COMPLETE) {
				printf("FB error, status: 0x%x\n", err);
			}
		}
		NormBuff = fbos[0];
		LightBuff = fbos[1];
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		//
	}

	GLuint NormBuff, LightBuff , DepthTex;
	static const u32 TexCnt = 2;
	GLuint Textures[TexCnt];

	Dis::ShaderProg* LightPass;
	Dis::ShaderProg* Post;
};

DefferedStuff * DS = null;

bool NormalPass = false; ///LAZY  --- todo



void DM_DefferedPrep::proc(RenderingCntx &rc) {
	//return;
	SetLastError(0);
	if(DS == null) DS = new DefferedStuff();
	
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, DS->NormBuff );

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	NormalPass =  true;

	

}

void DM_DefferedLightPass::proc(RenderingCntx &rc) {
	//return;

	SetLastError(0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, DS->LightBuff);
	
	//if( debugFlag(2) ) return;
	
	DS->LightPass->apply(rc, (mat4f)View, Identity() );

	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, DS->Textures[0] );

	glBegin(GL_TRIANGLES);                      
		glVertex3f(-1.0f, 3.0f, 0.5f);             
		glVertex3f(3.0f, -1.0f, 0.5f);             
		glVertex3f( -1.0f, -1.0f, 0.5f);             
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0 );
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, DS->NormBuff );
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, DS->Textures[1] );
	glActiveTexture(GL_TEXTURE0 );

	if(debugFlag(3) ) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0 );
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	NormalPass = false;

}


void DM_DefferedFinal::proc(RenderingCntx &rc) {
	
	SetLastError(0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0 );
	glActiveTexture(GL_TEXTURE0 );

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0 );
	

	if( debugFlag(2) ) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, DS->LightBuff);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, sSz.x, sSz.y, 0, 0, sSz.x, sSz.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0 );

		return;
	}
;
	//*
	glBindFramebuffer(GL_READ_FRAMEBUFFER, DS->NormBuff);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, sSz.x, sSz.y, 0, 0, sSz.x, sSz.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0 ); 
	/*
	DS->Post->apply(rc, (mat4f)View, Identity() );

	glBindTexture(GL_TEXTURE_2D, DS->Textures[0] );

	glEnable(GL_DEPTH_TEST);

	glBegin(GL_TRIANGLES);                      
		glVertex3f(-1.0f, 3.0f, 0.5f);             
		glVertex3f(3.0f, -1.0f, 0.5f);             
		glVertex3f( -1.0f, -1.0f, 0.5f);             
	glEnd();
	*/

	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


}

