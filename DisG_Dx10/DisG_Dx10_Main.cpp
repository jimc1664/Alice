#include "stdafx.h"

#include "Dx10_Main.h"

#include "Dx10_Task.h"

#include "Stdio.h"
#include "iostream"

#define PRINT(x) std::cout<<x
#define PRINTL(x) std::cout<<x<<"\n"

struct BasicErr {
	BasicErr( const char *const &str ) : Str(str) { }
	const char *Str;
	const BasicErr& print() const { PRINTL("  /Str:: "<<Str); return *this; }
};

struct GleamErr : public BasicErr {
	GleamErr( const char *const &file,  const u32 &line, const char *const &str, const HRESULT &hr ) : File(file), Line(line), BasicErr(str), HR(hr) {} 
	
	const GleamErr& print() { 
		PRINT("  /Str:: "<<Str);
		switch( HR ) {
			case D3D10_ERROR_FILE_NOT_FOUND					: PRINT( "  :D3D10_ERROR_FILE_NOT_FOUND The file was not found. ?? "); break;
			case D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS	: PRINT( "  :D3D10_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS There are too many unique instances of a particular type of state object. "); break;
//?			case D3DERR_INVALIDCALL							: PRINT( "  :D3DERR_INVALIDCALL The method call is invalid. For example, a method's parameter may not be a valid pointer. "); break;
//?			case D3DERR_WASSTILLDRAWING						: PRINT( "  :D3DERR_WASSTILLDRAWING The previous blit operation that is transferring information to or from this surface is incomplete. "); break;
			case E_FAIL										: PRINT( "  :E_FAIL . "); break;
			case E_INVALIDARG								: PRINT( "  :E_INVALIDARG An invalid parameter was passed to the returning function."); break;
			case E_NOINTERFACE								: PRINT( "  :E_NOINTERFACE No object interface is available.."); break;
			default											: PRINT( "  :Unknown error.");
		}
		PRINTL( "  /File:: "<<File<<"  /Line:: "<<Line); 
		return *this; 
	}

	const char *File; 
	const u32 Line; 
	const HRESULT HR;
}; 

#define PThrow(a) throw (*static_cast<const decltype(a)*>(&a.print() ))

#define GThrowFail(a,b)			PThrow ( GleamErr( __FILE__, __LINE__, b, a ) )
#define GThrowOnFAILED(a,b)		{ HRESULT hr = a; if(FAILED(hr)) GThrowFail(hr,b); }0
#define ShdrThrowOnFAILED(a,b,c)	{ HRESULT hr = a; if(b) { MessageBoxA( NULL, (char*)errors->GetBufferPointer(), "Shader Compile Error", MB_OK ); b->Release(); } if(FAILED(hr)) GThrowFail(hr,c); }0

#define S_RELEASE(a) if(a) { a->Release(); a = 0; }0

Dx10_Main::Dx10_Main() {
	//DXFactory = 0; Device = 0;

	DEFClassFunc( void, mainLoopWrp, Dx10_Main, mainLoop );
	MainThread.start( mainLoopWrp, this );
}
Dx10_Main::~Dx10_Main() {
	//S_RELEASE( Device );
	//S_RELEASE( DXFactory );
}

void Dx10_Main::mainLoop() {

	init();

	for( ;!Shutdown; ) {

		Tasks.proc( this );

		//printf( "Dx10_Main proc \n");
		Sleep(30 );
	}

	deinit();

}

void Dx10_Main::init() {

	Shutdown = 0;

	u32 createDeviceFlags = 0;
	HRESULT hr;
	D3D10_DRIVER_TYPE DriverType = D3D10_DRIVER_TYPE_HARDWARE;
	if( FAILED( hr = D3D10CreateDevice( NULL, DriverType, NULL, createDeviceFlags, D3D10_SDK_VERSION, &Device ) )){
		GThrowFail(hr,"Failed to create device");
		DriverType = D3D10_DRIVER_TYPE_REFERENCE;
		if( FAILED( hr = D3D10CreateDevice( NULL, DriverType, NULL, createDeviceFlags, D3D10_SDK_VERSION, &Device ) ) )  GThrowFail(hr,"Failed to create device");
		else PRINT("Device created in Reference mode\n");
	}

	GThrowOnFAILED( CreateDXGIFactory(  __uuidof( IDXGIFactory ), (void**)&DXFactory ),			"Failed to create factory" );

}
void Dx10_Main::deinit() {

	Device.release();
	DXFactory.release();
}


void Dx10_Main::task( Dx10_Task_Base *t ) {
	Singleton<Dx10_Main>::Ptr.Tasks.add( t );
}
using namespace Dis;

void DisMain::_construct() { Dx10_Main::construct(); }
void DisMain::_destruct() { Dx10_Main::destruct(); }

Alloc_Singleton( Dx10_Main );
DisMain& Singleton<DisMain>::Ptr = Dx10_Main::Singleton<Dx10_Main>::Ptr;

namespace Gem { 
namespace _Internal { 
void _INT_forceLink_Dis() {}
}
}