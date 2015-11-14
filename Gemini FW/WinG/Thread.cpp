#include "stdafx.h"

#include "Gem/ConCur/Thread.h"

struct ThreadDat {
	cb_Func Func;
	ptr Data;
	volatile bool Flag;
};

DWORD WINAPI ThreadFunc( void* d ) {
	auto &td = *(ThreadDat*)d;
	auto func = td.Func; auto dat = td.Data;
	td.Flag = 0;
	func( dat );
	return 0;
}

Thread::Thread() {
	Hndl = null;
}
Thread::~Thread() {
	if (Hndl != null) CloseHandle(Hndl);
}
void Thread::start( cb_Func func, void *d ) { 
	Assume( !Hndl );
	//Hndl = (HANDLE)_beginthread( func, 0, d );
//	PTHREAD_START_ROUTINE a;
	ThreadDat td = {
		func, d, 1
	};
	Hndl = CreateThread( 0, 0, ThreadFunc, &td, /*CREATE_SUSPENDED*/ 0, 0 );
	while( td.Flag ) Sleep(1);
	Assume( Hndl );
}
void Thread::waitFor( const u32 &time  ) {
	WaitForSingleObject( Hndl, time );
}
u32 Thread::id() { return GetCurrentThreadId(); }

__declspec(thread) ptr Thread_LocalData[4];

ptr& Thread::tld( const u32 &i ) { 
	return Thread_LocalData[i]; 
}

#include "Gem/ConCur/ConCur.h"


void ConCur::sleep( const u32 &ms) {
	Sleep(ms);
}
bool ConCur::compareSwap( volatile u32 &at, const u32 &with, const u32 &prev  ) {	 
	return InterlockedCompareExchange(&at, with, prev) == prev;
}

void ConCur::fullExit(int ec ) {
	ExitProcess(ec);
}