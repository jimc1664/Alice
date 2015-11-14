#ifndef GEM_BASIC_SINGLETON_H
#define GEM_BASIC_SINGLETON_H

#include "..\Basic.h"

namespace Gem {

template<class Obj > struct Singleton {
public:
	struct ScopeHandle {
		ScopeHandle(){ Singleton<Obj>::construct(); }
		~ScopeHandle(){ Singleton<Obj>::destruct(); }
		void shutdown() { Singleton<Obj>::shutdown(); }
	};
	static Obj& ptr();

	static void construct() { ptr()._construct(); }
	static void destruct()	{ ptr()._destruct(); }
	static void shutdown()	{ ptr()._shutdown(); }

protected:
	void _construct() { new( &ptr() ) Obj(); }
	void _destruct() { ptr().~Obj(); }
	void _shutdown() { }
};

#define Alloc_Singleton( Obj )	\
u8 _##Obj##_Space[sizeof(Obj)]; \
template<> Obj& Singleton<Obj>::ptr() { return *(Obj*)&_##Obj##_Space[0]; }

}


#endif //GEM_BASIC_SINGLETON_H