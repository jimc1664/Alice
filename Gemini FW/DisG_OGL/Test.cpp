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
//	return;
	/*
	auto &p = Plnt;

	u32 res = 1024, dim = 16, ic = dim*dim*6;
	
	if( !Init ) {
		Init = true;
		
		p.Res = res; p.Dim = dim; p.Res1 = p.Res +1; p.Dim1 = p.Dim +1;

		ary<PlanetDat> &pd = *(ary<PlanetDat> *)&p.Pd;
		getPlanet(res, pd );
		
		u32 stride = p.Res/dim;

		verts.addMul(  p.Dim1*p.Dim1);
		inds.enSpace( dim*dim*6 );
		ary<u8> imageBuf(p.Res*p.Res*3,p.Res*p.Res*3);

		for( int si = 6; si--; ) {	
			auto &s = p.Sides[si];
			u32 svi = si*p.Res1*p.Res1;
			s.Norm = pd[svi+ p.Res/2 + p.Res/2*p.Res1].Pos.getNormal();
			s.Corners[0] = pd[svi+ 0].Pos;
			s.Corners[1] = pd[svi+ p.Res].Pos;
			s.Corners[2] = pd[svi+ p.Res+p.Res*p.Res1].Pos;
			s.Corners[0] = pd[svi+ p.Res*p.Res1].Pos;

			
			s.init(p, svi, stride, 0, 0, (pd[svi+ p.Res/2 + p.Res/2*p.Res1].Pos- s.Corners[0]).leng()*2.3f );// (pd[svi+ p.Res/2 + p.Res/2*p.Res1].Pos - s.Corners[3]).leng()  );
			s.DesLod =  dim*dim*6;


			glGenTextures(1, &s.Tex);
			glBindTexture(GL_TEXTURE_2D, s.Tex);
        
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 	
			//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

			GLenum format = GL_RGB;
			GLenum intFormat = GL_RGB8;	
			u32 formatSize = 3;

			for( int x = p.Res; x--; )
			for( int y = p.Res; y--; ) {
				u8* pix = &imageBuf[(x+y*p.Res)*3];
				vec3u8 c = (vec3u8)(vec3s)(pd[si*p.Res1*p.Res1+ x+y*p.Res1].Col*255.0f);
				(*(vec3u8*)pix) = c;
			}

			glTexImage2D(
				GL_TEXTURE_2D, 0, intFormat, p.Res, p.Res,
				0, format, GL_UNSIGNED_BYTE, &imageBuf[0]
			);

			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
			//*  /
		

		}

	}
	
	//return;
	if(debugFlag(0)) {
		glDisable(GL_CULL_FACE);
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	if(debugFlag(1)) glDisable(GL_CULL_FACE);

	auto projMatrix = mat4f::projection(45.0f * DEG_TO_RAD, 1024.0f/768.0f,0.001f, 100.0f);  //perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	auto mvp = Trans.as<mat4f>()*projMatrix;// *worldMatrix;
	Prog.apply(rc, mvp);



	Tex.Hdwr->apply(rc);

	vec3f camPos = vec3f(0, 0, 0)* Trans.inverse();
	
	vec3f surfN = camPos.getNormal(), surfP = surfN*15.0f;
	//camPos = (camPos + surfP*3.0f)*0.25f;
	GLint camPosLocation = glGetUniformLocation(Prog.Prog, "CamPos");
	LodModLocation = glGetUniformLocation(Prog.Prog, "LodMod");
	glUniform3fv(camPosLocation, 1, &camPos.x );


	//debugPosition() = surfP;// p.normalise() *15.0f;
	//debugPosition() = P.Pd[2*p.Res1*p.Res1+ p.Res/2 + p.Res/2*p.Res1].Pos;
	static int iter = 0; iter++;

	

	for( int si = 6; si--; ) {	
		//si = 2;
		auto &s = p.Sides[si];
		
		//if( si == 2 ) debugPosition() = s.SubNodes[ (iter/1024) %4].Mid;// *Trans.inverse();
		//debugPosition() = vec3f(0, 0, 0) *Trans.inverse();

		//if(dot(s.Norm, s.Corners[0].getNormal()) >  dot(s.Norm, surfP.getNormal())) goto label_breakContinue;

		
		for( int i = 4; i--; ) {
			if(dot(surfN, s.Corners[i]) > 0) goto label_jumpOut;
		}
		continue;


		label_jumpOut:;
		if( si == 2 ) {
		//	printf("d %f  %f  %f  %f  \n", (s.SubNodes[0].Mid- camPos).leng(), (s.SubNodes[1].Mid- camPos).leng(), (s.SubNodes[2].Mid- camPos).leng(), (s.SubNodes[3].Mid- camPos).leng());
		}

		
		sList<PNode> drawList, procList; procList.add(&s);
		for(PNode *n; !procList.isEmpty();) {
			n = procList.pop();
			n->proc(p, drawList, procList, camPos);
		}
		if(drawList.isEmpty()) continue;

		glBindTexture(GL_TEXTURE_2D, s.Tex);
		for(PNode *n; !drawList.isEmpty();) {
			n = drawList.pop();
			n->draw(rc);
		}

	}



	glBindVertexArray(0);

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glEnable(GL_CULL_FACE); */
}
