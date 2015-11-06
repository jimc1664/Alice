#ifndef GEM_DIS_DX10_MAIN_H
#define GEM_DIS_DX10_MAIN_H

#include "../Dis/DisMain.h"
#include "Gem/ConCur/Thread.h"
#include "Gem/ConCur/TaskQ.h"

namespace Gem {  namespace Dx10 { 

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

class Dx10_Task_Base;

class Dx10_Main : public Dis::DisMain, public Singleton<Dx10_Main> {
public:

	static void task( Dx10_Task_Base *t );
private:
	Thread  MainThread;
friend class Dis::DisMain;
	static void construct() { new( &Singleton<Dx10_Main>::Ptr ) Dx10_Main(); }
	static void destruct() { Singleton<Dx10_Main>::Ptr.~Dx10_Main(); }


	Dx10_Main();
	~Dx10_Main();
	
	void init();
	void deinit();
	void mainLoop();

	volatile bool Shutdown;

	autoDXPtr<ID3D10Device*>        Device;
	autoDXPtr<IDXGIFactory*>		DXFactory;

	TaskQ		Tasks;
};

} }

#endif //GEM_DIS_DX10_MAIN_H