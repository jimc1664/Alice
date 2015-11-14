#include "stdafx.h"

#include "Gem/Dis/Camera.h"
#include "Gem/Dis/RenderTarget.h"
#include "Gem/Dis/DrawList.h"
#include "Gem/Dis/Texture.h"
#include "Gem/Dis/Mesh.h"
#include "Gem/Dis/ShaderProg.h"

#include "OGL_Task.h"

#include "Gem/ConCur/ConCur.h"

#include "Gem/Math/vec3.h"
#include "Gem/Math/vec4.h"
#include "Gem/Math/matrix4.h"
#include "Gem/Org/Ary.h"

#include "Gem/Scene3/TestObj.h"
#include "Gem/Scene3/Texture.h"
#include "Gem/Scene3/Passive.h"
#include "Gem/Scene3/Mesh.h"


struct Vertex2
{
  float x,y,z;  
};

Vertex2 verts[]={
	//Front Face
    {-0.5f, -0.5f, 0.5f},
     {0.5f, -0.5f, 0.5f},
     {0.5f,  0.5f, 0.5f},

	 { -0.5f, -0.5f, 0.5f },
	 {-0.5f, 0.5f, 0.5f},
	 { 0.5f, 0.5f, 0.5f },

	 //Back Face
	 { -0.5f, -0.5f, -0.5f },
	 { 0.5f, -0.5f, -0.5f },
	 { 0.5f, 0.5f, -0.5f },

	 { -0.5f, -0.5f, -0.5f },
	 { -0.5f, 0.5f, -0.5f },
	 { 0.5f, 0.5f, -0.5f },

	 //Left Face
   { -0.5f, -0.5f, -0.5f },
    { -0.5f, 0.5f, -0.5f },
    {-0.5f, -0.5f,  0.5f },

    { -0.5f, 0.5f, -0.5f },
    { -0.5f, 0.5f, 0.5f },
    { -0.5f, -0.5f, 0.5f },

	 //Right Face
   { 0.5f, -0.5f, -0.5f },
    { 0.5f, 0.5f, -0.5f },
    {0.5f, -0.5f,  0.5f },

    { 0.5f, 0.5f, -0.5f },
    { 0.5f, 0.5f, 0.5f },
    { 0.5f, -0.5f, 0.5f },

   //top Face
   {0.5f,  0.5f, 0.5f},
   {0.5f,  0.5f, -0.5f},
   {-0.5f,  0.5f, -0.5f},

   {-0.5f,  0.5f, -0.5f},
   {0.5f,  0.5f, 0.5f},
   {-0.5f,  0.5f, 0.5f},

	 //bottom Face
   {0.5f,  -0.5f, 0.5f},
   {0.5f,  -0.5f, -0.5f},
   {-0.5f,  -0.5f, -0.5f},

   {-0.5f,  -0.5f, -0.5f},
   {0.5f,  -0.5f, 0.5f},
   {-0.5f,  -0.5f, 0.5f},
   };


void DrawTestCube::proc(RenderingCntx &rc) {
//	return;

	/*

	if( VBO  == -1 ) {

		MeshData md;
		if( loadFBXFromFile( CSTR( "Media//armoredrecon.fbx" ), &md ) ) {

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, md.getNumVerts()*sizeof(Vertex), &md.vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (IC =  md.getNumIndices())*sizeof(int), &md.indices[0], GL_STATIC_DRAW);


	GLuint vertexShaderProgram = 0;
	CStr vsPath = CSTR("Media//Shaders//textureVS.glsl");
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);
	checkForCompilerErrors(vertexShaderProgram);

	GLuint fragmentShaderProgram = 0;
	CStr fsPath = CSTR("Media//Shaders//textureFS.glsl");
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);
	checkForCompilerErrors(fragmentShaderProgram);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderProgram);
	glAttachShader(shaderProgram, fragmentShaderProgram);

	//Link attributes
	glBindAttribLocation(shaderProgram, 0, "vertexPosition");
	glBindAttribLocation(shaderProgram, 1, "vertexColour");
	glBindAttribLocation(shaderProgram, 2, "vertexTexCoords");

	glLinkProgram(shaderProgram);
	checkForLinkErrors(shaderProgram);
	//now we can delete the VS & FS Programs
//	glDeleteShader(vertexShaderProgram);
//	glDeleteShader(fragmentShaderProgram);

		} else {

			ThrowAssert("err");

		}
	}
	glUseProgram(shaderProgram);


	auto projMatrix = mat4f::projection( 45.0f * DEG_TO_RAD, 1024.0f/768.0f,0.1f, 100.0f);  //perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	auto viewMatrix = mat4f::camLookAt(  vec3f(0.0f, 0.9f, -20.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f) );

	auto MVPMatrix = Trans.as<mat4f>()* viewMatrix*projMatrix;// *worldMatrix;


	GLint MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, MVPMatrix );
	GLint tex0Location = glGetUniformLocation(shaderProgram, "texture0");
	glUniform1i(tex0Location, 0 );

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3f)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void**)(sizeof(vec3f) + sizeof(vec4f)));
	
	glActiveTexture(GL_TEXTURE0);
	Tex.Hdwr->apply(dm);

	//glUseProgram(0);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, IC, GL_UNSIGNED_INT, 0);


	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);

	return;

	if( VBO  == -1 ) {
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(Vertex2), verts, GL_STATIC_DRAW);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO );

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 30);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	*/
}


void DrawPassive::proc(RenderingCntx &rc) {
//	return;
	auto projMatrix = mat4f::projection( 45.0f * DEG_TO_RAD, 1024.0f/768.0f,0.1f, 100.0f);  //perspective(45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
	auto viewMatrix = mat4f::camLookAt(  vec3f(0.0f, 0.9f, -20.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f) );

	auto mvp = Trans.as<mat4f>()* viewMatrix*projMatrix;// *worldMatrix;
	
	Dat.Prog.apply(rc, mvp);

	glActiveTexture(GL_TEXTURE0);
	Dat.Tex.Hdwr->apply(rc);

	Dat.Mesh.Hdwr->draw(rc);
}

