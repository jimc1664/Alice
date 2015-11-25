#include "../Gem/stdafx.h"

#include "JUI_Main.h"
#include <Gem/JUI/InputGroup.h>
#include <Gem/Basic/Math.h>


namespace Gem { namespace JUI {
vec2s16 mPos() {
	auto &ui = JUI_Main::ptr();
	return ui.MPos;
	//return *(vec2s16*)&ui._MPos;
}



bool key(  Keycode key ) {
	static_assert(_Keycode_Last <= 0xff, "Keycode  invalid ");
	auto &ui = JUI_Main::ptr();

	volatile bool ret = ui.key( (u8)key );
	return  ret;
	//return *(vec2s16*)&ui._MPos;
}
}	}

using namespace JUI;


void Toggle::proc(JUI_Main &cntx) {
	if( LKey != cntx.key(Key) ) {
		LKey = cntx.key(Key);
		if(!LKey) Value = !Value;
	}
}

void Toggle::deactivate() {	
	TaskSchedule(JUI_Main, {
		cntx.Coroutines.detach(this);
	} );
}

void Toggle::activate() {
	TaskSchedule(JUI_Main, {
		cntx.Coroutines.add(this);
	} );
}


template< int Dim> void Axis_T<Dim>::deactivate() {	
	TaskSchedule(JUI_Main, {
		cntx.Coroutines.detach(this);
	} );
}

template< int Dim> void Axis_T<Dim>::activate() {
	TaskSchedule(JUI_Main, {
		cntx.Coroutines.add(this);
	} );
}

template< int Dim> void Axis_T<Dim>::proc(JUI_Main &cntx) {
	
	for( int i = Dim; i--; ) {
		float des = 0.0f;
	
		if (cntx.key(Key[i*2])) des -= 1.0f;
		if (cntx.key(Key[i*2+1])) des += 1.0f;

		Value[i] += (des - Value[i])*min( cntx.DeltaTime*8.0f, 1.0f);
	}
}



Template1 void _foo2( T &a ) {
	a.activate(); a.deactivate();
}
void _foo() {
	_foo2(Axis());
	_foo2(AxisDual());
}
bool JUI_Main::key(const u8 &key) {
	return KeyData[key].Pressed;
}
bool JUI_Main::keyDown(const u8 &key) {

	KeyData[key].Pressed = true;
	return false;
}
bool JUI_Main::keyUp(const u8 &key) {

	KeyData[key].Pressed = false;
	return false;
}
bool JUI_Main::mMove(const vec2s16 &mm ) {
	_MPos = *(int*)&mm;
	return false;
}

void JUI_Main::update() {

	auto elap = LastUpdate.elap();
	if(elap.asI() < 8) return;
	*const_cast<float*>(&DeltaTime) = (f32)elap.asF();
	LastUpdate.update();

	for( auto &c : Coroutines ) 
		c._proc(*this);
	

	Tasks.proc( this );
}


JUI_Main::JUI_Main() : DeltaTime(0) {

}
JUI_Main::~JUI_Main() {
}

void JUI_Main::_shutdown() {

}

Alloc_Singleton(JUI_Main);