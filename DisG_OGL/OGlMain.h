#ifndef GEM_DIS_OGL_MAIN_H
#define GEM_DIS_OGL_MAIN_H

//#include "../Dis/DisMain.h"

//#define DisMain DisMainEx
//#include "Gem/Dis/DisMain.h"
//#undef DisMain 

#define GEM_DIS_MAIN_H

#include "Gem/ConCur/Thread.h"
#include "Gem/ConCur/TaskQ.h"
#include "Gem/Org/sList.h"

namespace Gem {  

namespace Dis { 
	class  MainTarget;
}
//using namespace Dis;

	
namespace Dis { 

Template1 struct autoDXPtr {
	autoDXPtr() : Ptr(0) {}
	~autoDXPtr() { if( Ptr ) release();  }
	operator T () { return Ptr; }

	T operator ->() { return Ptr; }
	T* operator &() { return &Ptr; }

	void release() { Ptr->Release(); Ptr = 0; }
private:
	T Ptr;
};

//class OGL_Task_Base;

///NOTE!!  - this deriviation is complex (hacky and fucked up..)
class DisMain : /*public DisMain,*/ public Singleton<DisMain>, public TaskHost<DisMain> {
public:
	static void construct();
	static void destruct();
	static void shutdown();
private: friend struct Singleton<DisMain>;
	Thread  MainThread;
//friend class DisMainEx;
	//static void construct() { new( &Singleton<DisMain>::ptr() ) DisMain(); }
	//static void destruct() { Singleton<DisMain>::ptr().~DisMain(); }


	DisMain();
	~DisMain();
	
	void _shutdown();
	void init();
	void deinit();
	void mainLoop();

	volatile bool Shutdown;

	//autoDXPtr<ID3D11Device*>        Device;
//	autoDXPtr<IDXGIFactory*>		DXFactory;


	Dis::MainTarget * Primary;
	friend class Dis::MainTarget;
	sList<Dis::MainTarget>	Outputs;

	HGLRC Cntx;
	//Texture BlankTex;
};



typedef DisMain OGlMain ;


} }


#endif //GEM_DIS_OGL_MAIN_H