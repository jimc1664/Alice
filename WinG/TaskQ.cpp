#include "stdafx.h"

#include "Gem/ConCur/TaskQ.h"

TaskQ::TaskQ() {
	static_assert( sizeof( _Space ) == sizeof(Head ), "TaskQ mismatch - header size mismatch" );
	//static_assert( MEMORY_ALLOCATION_ALIGNMENT == 8, "TaskQ mismatch - MEMORY_ALLOCATION_ALIGNMENT" );
	InitializeSListHead( &Head );
}
TaskQ::~TaskQ() {
	for( PSLIST_ENTRY  a = InterlockedFlushSList( &Head ); a; ) {
		Task *t = static_cast<Task*>((_TaskLink*)a);
		a = t->Entry.Next;
		t->destruct();
		t->freeMem();
	}
}

void TaskQ::add( Task *t ) {
	InterlockedPushEntrySList( &Head, &t->Entry  );
}
void TaskQ::proc( ptr context ) {
	for( PSLIST_ENTRY  a = InterlockedFlushSList( &Head ); a; ) {
		Task *t = static_cast<Task*>((_TaskLink*)a);
		a = t->Entry.Next;
		t->proc( context );
		//delete t;
	}
}
void TaskQ::procFwd( ptr context ) {
	if( PSLIST_ENTRY  a = InterlockedFlushSList( &Head ) ) {
		PSLIST_ENTRY prev = 0;
		for( ;; ) {
			Task *t = static_cast<Task*>((_TaskLink*)a);
			PSLIST_ENTRY c = a;
			a = t->Entry.Next;
			t->Entry.Next = prev;
			if( !a ) { a = c; break; }
			prev = c;
		}		
		for( ;; ) {
			Task *t = static_cast<Task*>((_TaskLink*)a);
			a = t->Entry.Next;
			t->proc( context );
			//delete t;
			if(!a ) break;
		}		
	}
}
bool TaskQ::procOne( ptr context ) {
	if( PSLIST_ENTRY a = InterlockedPopEntrySList( &Head ) ) {
		Task *t = static_cast<Task*>((_TaskLink*)a);
		t->proc( context );
		//delete t;
		return true;
	} else return false;
}

ptr TaskQ::Task::alloc(sizet s) {
	return _aligned_malloc(s,MEMORY_ALLOCATION_ALIGNMENT);
}

void TaskQ::Task::freeMem() {
	_aligned_free(this);
}

//TaskQ::Task::~Task(){}