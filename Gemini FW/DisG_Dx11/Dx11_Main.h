#ifndef GEM_DIS_DX11_MAIN_H
#define GEM_DIS_DX11_MAIN_H

#include "../Dis/DisMain.h"
#include "Gem/ConCur/Thread.h"
#include "Gem/ConCur/TaskQ.h"

namespace Gem {  namespace Dx11 { 

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

class Dx11_Task_Base;

class Dx11_Main : public Dis::DisMain, public Singleton<Dx11_Main> {
public:

	static void task( Dx11_Task_Base *t );
private:
	Thread  MainThread;
friend class Dis::DisMain;
	static void construct() { new( &Singleton<Dx11_Main>::Ptr ) Dx11_Main(); }
	static void destruct() { Singleton<Dx11_Main>::Ptr.~Dx11_Main(); }


	Dx11_Main();
	~Dx11_Main();
	
	void init();
	void deinit();
	void mainLoop();

	volatile bool Shutdown;

	autoDXPtr<ID3D11Device*>        Device;
	autoDXPtr<IDXGIFactory*>		DXFactory;

	TaskQ		Tasks;
};

} }

#endif //GEM_DIS_DX11_MAIN_H