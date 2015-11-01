#ifndef GEM_JUI_MAIN_H
#define GEM_JUI_MAIN_H

#include "Gem/Basic/Singleton.h"
#include <Gem/JUI/JUI.h>
#include <Gem/OS/Time.h>
#include <Gem/Math/vec2.h>
#include <Gem/ConCur/TaskQ.h>

namespace Gem { namespace JUI {

class JUI_Main;

class KeyCallBack : public dListNode<KeyCallBack> {
public:
	typedef bool (*CBFunc)( void* );
	typedef bool (*CBFunc2)( void*, JUI_Main * );
	~KeyCallBack();
	KeyCallBack() : List(0) {}
	void activateUp( JUI_Main &cn, const u8 &key, const s32 &layer, const CBFunc2 &f, void* d );
	void activateDown( JUI_Main &cn, const u8 &key, const s32 &layer, const CBFunc2 &f, void* d );
	void activateUp( JUI_Main &cn, const u8 &key, const s32 &layer, const CBFunc &f, void* d )	{ activateUp(cn,  key,layer,(CBFunc2)f,d); }
	void activateDown( JUI_Main &cn, const u8 &key, const s32 &layer, const CBFunc &f, void* d )	{ activateDown( cn, key,layer,(CBFunc2)f,d); }
	//void activateUp( const u8 &key, const s32 &layer, const CB<CBFunc2> &f )	{ activateUp( key,layer,f.Func,f.d); }
	void deactivate( JUI_Main &cn );
	void destroy( ) { Assume(0); }
	void int_Destroy( JUI_Main &cn ) {
		//Assume(!List);
		deactivate(cn); delete this;
	}
protected: friend class JUI_Main;
	//KeyCallBack( const s32 &layer, const CBFunc &f, void* d, dList<KeyCallBack> &head ) : Func(f), Data(d), Layer(layer) { head.insertSorted( this ); }
	//KeyCallBack( const s32 &layer, const CBFunc &f, void* d ) : Func(f), Data(d), Layer(layer) { }

	friend bool operator > ( const KeyCallBack &a, const KeyCallBack &b ) { return a.Layer < b.Layer; }
	//virtual void destroy() { unlink(); delete this; }

	CBFunc2 Func; void* Data;
	u32	   Layer; 
	dList<KeyCallBack> *List; 
};

class JUI_Task_Base;

class JUI_Main : public Singleton<JUI_Main>, public TaskHost<JUI_Main> {
public:

	bool keyDown(const u8 &key);
	bool keyUp(const u8 &key);
	bool mMove( const vec2s16 &mp );

	void update();

	bool key(const u8 &key);

	dList<JUI_Coroutine_Base> Coroutines;

	const f32 DeltaTime; 

	union {
		struct { vec2s16  MPos; };
		volatile int _MPos;
	};

private: friend struct Singleton<JUI_Main>;

	struct KeyDataS {
		KeyDataS() : Pressed(false) {}
		dList<KeyCallBack> KeyDown;
		dList<KeyCallBack> KeyUp;
		bool Pressed;
	};	
	KeyDataS				KeyData[0xff];

	OS::Time LastUpdate;
	
	JUI_Main();
	~JUI_Main();
	void _shutdown();


};
class JUI_Task_Base : public TaskQ::Task { };

Template1 class JUI_Task : public TaskHlpr<T,JUI_Task_Base> {};

} }

#endif //GEM_INTERNAL_MAIN_H