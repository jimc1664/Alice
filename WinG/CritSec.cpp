#include "stdafx.h"

#include "Gem/ConCur/CritSec.h"


CritSec::CritSec() {
//	static_assert( sizeof(_Space ) == sizeof(CRITICAL_SECTION), "Err- crit sec size mismatch"  );
	InitializeCriticalSection( &Hndl );
}
CritSec::~CritSec() {
	DeleteCriticalSection(&Hndl );
}
void CritSec::lock() {
	EnterCriticalSection( &Hndl );
}
//TryEnterCriticalSection 
void CritSec::unlock() {
	LeaveCriticalSection( &Hndl );
}

