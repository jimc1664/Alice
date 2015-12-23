#include "../stdafx.h"

#include <Gem/ConCur/WorkHost.h>
#include <Gem/ConCur/ConCur.h>

#include <thread> // std::thread::hardware_concurrency();
 

WorkHost::WorkHost( const u32 &threadCount ) {
	u32 tc = threadCount;
	if(tc == 0) tc = std::thread::hardware_concurrency();
	if(tc == 0) tc = 4; //guess...

	MaxThreads = tc;
	
	Running = true;
	DEFClassFunc( void, mainLoopWrp, WorkHost, mainLoop );
	Threads.addMul(tc);
	for(int i = tc; i--;)
		Threads[i].start(mainLoopWrp, this);

}

void WorkHost::_shutdown() {
	Running = false;
}
WorkHost::~WorkHost() {
	Running = false;
	for(int i = Threads.count(); i--;)
		Threads[i].waitFor();
}



void WorkHost::mainLoop() {

	for(;;) {
		if( !Tasks.procOne(this) ) {
			if( !Running ) {
				ConCur::sleep(5);
				if( !Tasks.procOne(this) && !Running ) {
					return;
				}
			} else 
				ConCur::sleep(1); //todo - only need one thread to be spinning
		}
		
	}

}


Alloc_Singleton(WorkHost);