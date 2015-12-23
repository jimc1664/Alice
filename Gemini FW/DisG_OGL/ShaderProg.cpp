#include "stdafx.h"

#include "Gem/Dis/ShaderProg.h"

#include "OGl_Task.h"

#include "Gem/String/DStr.h"
#include "Gem/String/WStr.h"
#include "Gem/Org/dlist.h"

#include <Gem/ConCur/WorkHost.h>
#include <Gem/ConCur/Thread.h>


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
GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType);

namespace Gem {
class Rsrc_ParsedShader  : public Resource_T <Rsrc_ParsedShader> {
friend class Entry;

	Rsrc_ParsedShader( Entry &e, const u32 s ) : Resource_T(e), Sz(s) {}

/*
	void process( CallBackHndl &&cb2 ) {

		CStr fileC = Dat->str();
		int a = 0;
		printf("Worker %d :: parsing file %s \n", Thread::id(), Ent.Name.str());
	//	for( u32 i = 0; i < parts.count(); i++ ) {

		CallBackHndl cb =  Nothing();
		auto& p = fileC;// parts[i];
		for( CStr find = p; find.search('#', find ); ) {
			try {
				find.decStart(); 
				CStr word = find.nextWord();
				
				if( word.isEqual( CSTR("include"), true ) )  {
					CStr prm = find; prm.decStart(word.len()+1);
					prm = prm.getParam('"');	
					WStr<MAX_PATH> fn = Ent.Name.getPath() + prm;

					if( ! (bool)cb ) {
						cb = newCallBack(
							int a = 0;
						);
					}

					auto h = Rsrc_ParsedShader::fromFile((CStr)fn, cb.move() );
					int a = 0;
				} 
				
			}
			catch(...) {
				printf( "Failed to parse file... %s, \n ", Ent.Name.str() );
				return;
			}
			
			int a = 0;
		}

		Ent.Res = this;
		//} * /
	}
	//Rsrc_Raw::Hndl Dat;
	*/
	
	const u32 Sz;
	char Data[1];

	static void fromFile( WorkHost&, Entry &e, CallBackHndl &&uCb  ) {
		
		struct WrkSpc { 

			WrkSpc(  ):  UpperCB( Nothing() ) { }
			Rsrc_Raw::Hndl Dat; 
			CStr Header;
			struct Section {
				Rsrc_ParsedShader::Hndl Sub;
				CStr Str;
			};
			
			ary<Section> Sections;

			Entry* Ent;
			
			CallBackHndl UpperCB;
			
		
			void proc(  ) {

				Header = Dat->str();
				int a = 0;
				printf("Worker %d :: parsing file %s \n", Thread::id(), Ent->Name.str());
			//	for( u32 i = 0; i < parts.count(); i++ ) {

				CallBackHndl cb =  Nothing();
				CStr * p = &Header;// parts[i];
				for( CStr find = *p; find.search('#', find ); ) {
					try {
						//find.decStart(); 
						find.decStart();
						CStr word = find.nextWord();
				
						if( word.isEqual( CSTR("include"), true ) )  {
							CStr prm = find; prm.decStart(word.len()+1);
							prm = prm.getParam('"');	
			
							CStr n = p->trimmedStartTo(prm.end()+1 );
							*p = p->trimmedTo( find.start()-1 );
							//DStr t = *p;

							auto &ns = Sections.add();
							ns.Str = n;
							p = &ns.Str;

							WStr<MAX_PATH> fn = Ent->Name.getPath() + prm;
							if( ! (bool)cb ) {
								cb = newCallBack(this->finalise(););
							}
						
							ns.Sub = Rsrc_ParsedShader::fromFile((CStr)fn, cb.copy() );
						} 
				
					}
					catch(...) {
						printf( "Failed to parse file... %s, \n ", Ent->Name.str() );
						return;
					}
			
					int a = 0;
				}
				*p = p->trimmedDeadSpace();
				if(!cb) finalise();
			} 
			void finalise() {

				u32 cnt = Header.len();
				for(u32 i = Sections.count(); i--;)
					cnt += Sections[i].Str.len()+ Sections[i].Sub->Sz;
		
					
				u32 allc = sizeof(Rsrc_ParsedShader)-1 + cnt +1;
				Rsrc_ParsedShader& rs = *(Rsrc_ParsedShader*)malloc(allc); // memset(&rs, 'a', allc);
				
				new (&rs)Rsrc_ParsedShader(*Ent, cnt);		

				char * p = const_cast<char*>(&rs.Data[0]);

				Header.copyTo(p); p += Header.len(); //functionify todo
				for( u32 i = 0; i < Header.len(); i++ ) {
					char a = Header[i];
					int b = 0;
				}
				for( u32 i = 0; i < Sections.count(); i++ ) {
					auto &s = Sections[i];
					s.Sub->str().copyTo(p); p += s.Sub->str().len();
					s.Str.copyTo(p); p += s.Str.len();
				}
				*p = 0; //null terminate for convience  ( length() does not include null termination )
				Ent->Res = &rs;
				delete this;
			}
			void init( Entry &e, CallBackHndl &&uCb, Rsrc_Raw::Hndl &&h ) {
				Ent = &e; Dat = h; UpperCB = uCb.move();
			}
	
		};

		auto ws = new WrkSpc;
		auto cb = newCallBack( ws->proc(); );
		ws->init( e, uCb.move(), Rsrc_Raw::fromFile(e.Name, cb.copy() ) );
		//new Rsrc_ParsedShader(e, cb.moveHlpr() );
	}
public:
	CStr str() const { return CStr(Data, Sz); }
	static Hndl fromFile(const CStr &s, CallBackHndl cb = Nothing() ) {  return Entry::fromFile(s, cb.move() ); }
};
}


CritSec cs;
void ShaderProg::fromFile( WorkHost &, const CStr &_vs, const CStr &_ps) {

	DStr vs(_vs), ps(_ps);
	//CritSec::Handle h(cs);
	printf("Worker %d :: loading ShaderProg %s  -- %s \n", Thread::id(), vs.str(), ps.str());
		
	Rsrc_ParsedShader::Hndl Vs;
	
	struct Prm { Rsrc_ParsedShader::Hndl Vs, Ps; };
	auto cb = newCallBackPrm(Prm,

		TaskSchedule( DisMain, 
			printf("Worker %d :: compiling ShaderProg %s  -- %s \n", Thread::id(), vs.str(), ps.str());
	
			//p.Vs
			GLuint vertexShaderProgram = 0;
			vertexShaderProgram = loadShaderFromMemory( p.Vs->str(), VERTEX_SHADER);
			checkForCompilerErrors(vertexShaderProgram);

			GLuint fragmentShaderProgram = 0;
			fragmentShaderProgram = loadShaderFromMemory( p.Ps->str(), FRAGMENT_SHADER);
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

			glUseProgram(Prog);
			MVPLocation = glGetUniformLocation(Prog, "MVP");
			MVLocation = glGetUniformLocation(Prog, "MV");
			GLint tex0Location = glGetUniformLocation(Prog, "texture0");
			glUniform1i(glGetUniformLocation(Prog, "texture0"), 0 );
			GLint tex8Location = glGetUniformLocation(Prog, "LightBuff");
			glUniform1i(glGetUniformLocation(Prog, "LightBuff"), 1 );
			int loc = glGetAttribLocation( Prog, "vertexPosition");
			loc = glGetAttribLocation( Prog, "vertexColour");
			loc = glGetAttribLocation( Prog, "vertexTexCoords");
			loc = glGetAttribLocation( Prog, "vertexNorm");

			glUseProgram(0);
		);
	);

	cb->Vs = Rsrc_ParsedShader::fromFile(vs, cb.copy() );
	cb->Ps = Rsrc_ParsedShader::fromFile(ps, cb.copy() );
//	auto p = Rsrc_Raw::fromFile(ps);

	
	//now we can delete the VS & FS Programs
//	glDeleteShader(vertexShaderProgram);
//	glDeleteShader(fragmentShaderProgram);
}

void ShaderProg::apply(RenderingCntx &rc, const mat4f &mvp, const mat4f &mv ) {

	//todo - constant buffer   --- ogl -?-?- unifiorm buffer object ??
	glUseProgram(Prog);

	glUniformMatrix4fv( MVPLocation, 1, GL_FALSE, mvp );
	glUniformMatrix4fv( MVLocation, 1, GL_FALSE, mv );

}

ShaderProg* ShaderProg::fromFile(const CStr &vs, const CStr &ps) {

	auto ret = new ShaderProg;
	DStr vs2 = vs; 
	DStr ps2 = ps; 

	TaskSchedule( WorkHost, ret->fromFile(cntx,vs2, ps2) );
	return ret;
}


#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;


//Load it from a memory buffer
GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType) {
  GLuint program = glCreateShader(shaderType);
  glShaderSource(program, 1, &pMem, NULL);
  glCompileShader(program);
  return program;
}

struct ParseNode : dListNode<ParseNode> {

	CStr Full;

	dList<ParseNode> Children;
};


//Load Shader from File
GLuint loadShaderFromFile(const CStr& filename, SHADER_TYPE shaderType) {
	// string fileContents;
	
	for(;; ) {

	}
	CStr fn = filename;
	ary<CStr> parts; parts.add(CStr());
	ifstream file;

	for(int i = 0; ;) {
		file.open(fn.str(), std::ios::in);
		if (!file) {
			cout << "File "<<fn.str()<<" could not be loaded" << endl;
			return 0;
		}

		//calculate file size
		if (file.good()) {
			file.seekg(0, std::ios::end);
			u32 len = (u32)file.tellg();
			file.seekg(std::ios::beg);
			if (len == 0)  {
				std::cout << "File has no contents " << std::endl;
				return 0;
			}

			DStr fileC = DStr( Reserve(len+1) );
			//fileContents.resize(len);
			memset(fileC.str(), 0, len+1);	
			file.read( fileC.str() , len);
	
			file.close();

			parts.add(fileC);

			for( u32 i = 0; i < parts.count(); i++ ) {
				auto& p = parts[i];	
				for( CStr find = p; find.search('#', find ); ) {
					try {
						find.decStart(); 
						CStr word = find.nextWord();
				
						if( word.isEqual( CSTR("include"), true ) )  {
							CStr prm = find; prm.decStart(word.len()+1);
							prm = prm.getParam('"');					
							int a = 0;
						} 
				
					}
					catch(...) {
						std::cout << "Failed to parse file... " << std::endl;
						return 0;
					}
			
					int a = 0;
				}
			}
		}
	}

	if( parts.count() ==1 ) {
		GLuint program = loadShaderFromMemory(parts[0].str(), shaderType);
		return program;
	} else {



		GLuint program = loadShaderFromMemory(parts[0].str(), shaderType);
		return program;
	}
	//fileContents.
	
}

bool checkForCompilerErrors(GLuint shaderProgram) {
	GLint isCompiled = 0;
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)  {
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

bool checkForLinkErrors(GLuint program) {
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		//The maxLength includes the NULL character
		string infoLog; infoLog.resize(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
		cout << "Shader not linked " << infoLog << endl;
		//We don't need the shader anymore.
		glDeleteProgram(program);
		return true;
	}
	return false;
}


