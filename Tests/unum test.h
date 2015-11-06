
#include <Gem/Main.h>
#include <Gem/ConCur/Thread.h>

#include <stdio.h>

using namespace Gem;


struct Window {
	Window( int a, int b ) {

	}
	void doStuff() {
		int a =0;
	}
	int x;
};

class Main { 
friend class Gem::Main_Hlpr;
	Main() {
		int x = a +10;
		int y = 10 + a;
	//	int b = 0;

		w.doStuff();
			
		//printf(" t %d %x \n", Thread::id(), Thread::tld() );

		DEFClassFunc( void, mainLoopWrp, Main, mainLoop );
		T1.start( mainLoopWrp, this );

		//DEFClassFunc( void, mainLoopWrp, Main, mainLoop );
		T2.start( mainLoopWrp, this );	
	}

	void mainLoop( ) {
		printf(" t %d %x \n", Thread::id(), Thread::tld() );
	}

	AutoCtor( Window, w,	( 10, 5 ) );
	AutoCtor( int, a,		( 10 ) );

	void proc() {
		
		printf("hey ho mark2\n"); 

		for( ;; ) {

		}
	}	
	Thread T1;
	Thread T2;
};

DEF_MainClass( Main );

