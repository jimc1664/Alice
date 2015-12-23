#ifndef GEM_CONCUR_REFCOUNTER_H
#define GEM_CONCUR_REFCOUNTER_H


#include "../Basic.h"

class RefCounter { // : public Uncopyable {
public:
	RefCounter(const u32 start = 0 ) : Reference(start) {}
	u32 incRef(); //returns ref before inc
	bool decRef();  //returns true if this was the last reference
private:
	_declspec(align( 32 ) ) volatile u32 Reference;

	SetUncopyable(RefCounter);
};

#endif //GEM_CONCUR_REFCOUNTER_H