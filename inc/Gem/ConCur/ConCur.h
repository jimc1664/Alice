#ifndef GEM_CONCUR_CONCURL_H
#define GEM_CONCUR_CONCURL_H

#include "../Basic.h"

namespace Gem { 

class ConCur {
public:
	static void sleep( const u32 &ms);
	static void fullExit(int ); //todo find proper place
	static bool compareSwap( volatile u32 &at, const u32 &with, const u32 &prev );


};



}

#endif //GEM_CONCUR_CONCURL_H