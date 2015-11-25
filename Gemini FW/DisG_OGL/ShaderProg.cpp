#include "stdafx.h"

#include "Gem/Dis/ShaderProg.h"

#include "OGl_Task.h"

#include "Gem/String/DStr.h"




void checkError();

ShaderProg::ShaderProg() {
	Prog = 0;
}


enum SHADER_TYPE {
	VERTEX_SHADER=GL_VERTEX_SHADER,
	FRAGMENT_SHADER=GL_FRAGMENT_SHADER
};

GLuint loadShaderFromFile(const CStr& filename, SHADER_TYPE shaderType);
bool checkForCompilerErrors(GLuint shaderProgram);
bool checkForLinkErrors(GLuint program);


void ShaderProg::fromFile( DisMain &dm, const CStr &vs, const CStr &ps) {

	GLuint vertexShaderProgram = 0;
	vertexShaderProgram = loadShaderFromFile(vs, VERTEX_SHADER);
	checkForCompilerErrors(vertexShaderProgram);

	GLuint fragmentShaderProgram = 0;
	fragmentShaderProgram = loadShaderFromFile(ps, FRAGMENT_SHADER);
	checkForCompilerErrors(fragmentShaderProgram);

	Prog = glCreateProgram();
	glAttachShader(Prog, vertexShaderProgram);
	glAttachShader(Prog, fragmentShaderProgram);

	//Link attributes
	glBindAttribLocation(Prog, 0, "vertexPosition");
	glBindAttribLocation(Prog, 4, "vertexPosition2");
	glBindAttribLocation(Prog, 1, "vertexColour");
	glBindAttribLocation(Prog, 2, "vertexTexCoords");
	glBindAttribLocation(Prog, 3, "vertexNorm");


	glLinkProgram(Prog);
	checkForLinkErrors(Prog);

	GLint tex0Location = glGetUniformLocation(Prog, "texture0");
	glUniform1i(tex0Location, 0 );

	int loc = glGetAttribLocation( Prog, "vertexPosition");
	loc = glGetAttribLocation( Prog, "vertexColour");
	loc = glGetAttribLocation( Prog, "vertexTexCoords");
	loc = glGetAttribLocation( Prog, "vertexNorm");
	//now we can delete the VS & FS Programs
//	glDeleteShader(vertexShaderProgram);
//	glDeleteShader(fragmentShaderProgram);
}

void ShaderProg::apply(RenderingCntx &rc, const mat4f &mvp) {

	//todo - constant buffer   --- ogl -?-?- unifiorm buffer object ??
	glUseProgram(Prog);
	GLint MVPLocation = glGetUniformLocation(Prog, "MVP");
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, mvp );

}

ShaderProg* ShaderProg::fromFile(const CStr &vs, const CStr &ps) {

	auto ret = new ShaderProg;
	DStr vs2 = vs; 
	DStr ps2 = ps; 

	TaskSchedule( DisMain, ret->fromFile(cntx,vs2, ps2) );
	return ret;
}


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;


//Load it from a memory buffer
GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType)
{
  GLuint program = glCreateShader(shaderType);
  glShaderSource(program, 1, &pMem, NULL);
  glCompileShader(program);
  return program;
}

//Load Shader from File
GLuint loadShaderFromFile(const CStr& filename, SHADER_TYPE shaderType) {
  string fileContents;
  ifstream file;
  file.open(filename.str(), std::ios::in);
  if (!file)
  {
	  cout << "File could not be loaded" << endl;
    return 0;
  }

  //calculate file size
  if (file.good())
  {
    file.seekg(0, std::ios::end);
    u32 len = (u32)file.tellg();
    file.seekg(std::ios::beg);
    if (len == 0)
    {
      std::cout << "File has no contents " << std::endl;
      return 0;
    }

    fileContents.resize(len);
    file.read(&fileContents[0], len);
    file.close();
    GLuint program = loadShaderFromMemory(fileContents.c_str(), shaderType);
    return program;
  }

  return 0;
}

bool checkForCompilerErrors(GLuint shaderProgram)
{
  GLint isCompiled = 0;
  glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
    //The maxLength includes the NULL character
    string infoLog;
    infoLog.resize(maxLength);
    glGetShaderInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
    cout << "Shader not compiled " << infoLog << endl;
    //We don't need the shader anymore.
    glDeleteShader(shaderProgram);
    return true;
  }

  return false;
}

bool checkForLinkErrors(GLuint program)
{
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    //The maxLength includes the NULL character
    string infoLog;
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
    cout << "Shader not linked " << infoLog << endl;
    //We don't need the shader anymore.
    glDeleteProgram(program);
    return true;
  }
  return false;
}


