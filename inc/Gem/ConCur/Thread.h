#ifndef GEM_CONCUR_THREAD_H
#define GEM_CONCUR_THREAD_H

#include "../Basic.h"

namespace Gem { 

typedef void (*cb_Func) (ptr);

class Thread {
public:
	Thread();
	~Thread();
	void start(cb_Func func, ptr d);
	void waitFor(const u32 &time = -1 );
	static u32 id();
	static ptr& tld(const u32 &i = 0);
private:	
	void* Hndl;
};

}	

#endif //GEM_CONCUR_THREAD_H