#ifndef GEM_CONCUR_ATOMIC_H
#define GEM_CONCUR_ATOMIC_H

#include "../Basic.h"

Template1 class AtomicT {
public:
	AtomicT() {}
	AtomicT( const T&v ) : Val(v) {}
	operator T()	const  { return Val; }
	T get()			const  { return Val; }
	T increment();
	T decrement();
	bool trySet( const T &a );
	bool trySet( const T &a, const T &b ); //b = original value (if a is dependan on Val )
private:
	_declspec(align( 32 ) ) volatile T Val;
	SetUncopyable(AtomicT);
};

typedef AtomicT<u32> Atomic32;
typedef AtomicT<u64> Atomic64;
typedef Atomic32 Atomic;

#endif //GEM_CONCUR_ATOMIC_H