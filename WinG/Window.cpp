#include "stdafx.h"
#include "Gem/OS/Window.h"

Window::Window( const char * caption, const vec2u16 &size, const u32 &flags ) : Size(size) {	
	DWORD style = WS_SYSMENU| WS_MINIMIZEBOX | WS_BORDER | WS_CAPTION | WS_VISIBLE | CS_DBLCLKS;	//WS_OVERLAPPEDWINDOW
	if( flags & Flg_Resizeable ) style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
	RECT rc = { 0, 0, size.x,size.y };
	AdjustWindowRect( &rc, style, FALSE );
	Hndl = CreateWindowA( "AnAwesomeWindowClassName", caption, style, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, 0, NULL ); 
	Assume( Hndl );
	

//	HRESULT hr = RegisterDragDrop( getHndl(), &DH );	Assume( hr == S_OK );

	SetWindowLongPtr( Hndl, GWLP_USERDATA, (size_t)this );
}
