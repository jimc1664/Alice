#ifndef GEM_MAIN_H
#define GEM_MAIN_H

#include "Basic.h"
namespace Gem {

class Main_Hlpr {
protected:
	static			void	construct();
	static			void	destruct();
	static			void	shutdown();
private:
	static			u8		Space[];
};

namespace _Internal { void _INT_forceLink_Gemini(); }

#define DEF_MainClass(mainClass) \
void				Main_Hlpr::construct()							{ new (Space) mainClass();								} \
void				Main_Hlpr::destruct()							{ ((mainClass*)Space)->~mainClass(); } \
void				Main_Hlpr::shutdown()							{ ((mainClass*)Space)->shutdown(); } \
u8					Main_Hlpr::Space[sizeof(mainClass)];  \
void _forceLink() { _Internal::_INT_forceLink_Gemini(); }

}
/*
class MainProc {
protected:
	static			void	construct();
	static			void	destruct();
	static			void	loop();
	//static 	ptr		get()		{ return Space; }
	//static			void	fileDropped( const CStr& fileName );
	//static			void	SetCB( const u32 &i, const func &f );
	//static			bool	KeyState( const u8 &key );
private:
	static			u8		Space[];
}; 

void _forceLink_Gemini();

#define DefMain(mainClass) \
void				MainProc::construct()							{ new (Space) mainClass();								} \
void				MainProc::destruct()							{ ((mainClass*)Space)->~mainClass(); } \
void				MainProc::loop()								{ ((mainClass*)Space)->proc();						} \
u8					MainProc::Space[sizeof(mainClass)];  \
void _forceLink() { _forceLink_Gemini(); }

//void				Main::fileDropped( const CStr& fileName )	{ ((mainClass*)Space)->fileDropped(fileName);			} \


*/

#pragma comment( lib,LibName("Gem") )

#endif GEM_MAIN_H