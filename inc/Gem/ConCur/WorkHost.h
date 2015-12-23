#ifndef GEM_CONCUR_WORKHOST_H
#define GEM_CONCUR_WORKHOST_H

#include "TaskQ.h"
#include "Thread.h"
//#include "CritSec.h"
#include "../Org/Ary.h"

namespace Gem { 

class WorkHost : public TaskHost<WorkHost>, public Singleton<WorkHost> {
public:
	WorkHost( const u32 &threadCount = 0 );
	~WorkHost();


private: friend struct Singleton<WorkHost>;
	void _shutdown();
	void mainLoop();

	volatile bool Running;
	//CritSec Lock;
	u32 MaxThreads;

	ary<Thread> Threads;
};

}	

#endif //GEM_CONCUR_WORKHOST_H