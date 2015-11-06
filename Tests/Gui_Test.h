

#include "Gem/Main.h"
#include "Gem/MainWindow.h"
//#include <Gem/ConCur/Thread.h>

#include <stdio.h>

using namespace Gem;



class Main { 
friend class Gem::Main_Hlpr;
	Main() {
			
	}
	//MainWindow w;
	AutoCtor( MainWindow, Wndw,	( "Window", vec2u16(1024,768), OS::Window::Flg_Resizeable ) );
};

DEF_MainClass( Main );
