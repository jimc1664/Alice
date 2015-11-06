#include "stdafx.h"

#include "InitConsole.h"

#include "Gem/Main.h"
#include "Gem/OS/Time.h"

#include "../Gem/Main_Internal.h"
#include "../JUI/JUI_Main.h"

#include <Gem/OS/DuctTape.h>

#pragma warning(disable :  4073)
#pragma init_seg( lib  )

namespace Gem { namespace OS {

class TimeInitator {
public:
	TimeInitator() {
		LARGE_INTEGER hpcfreq;
		QueryPerformanceFrequency(&hpcfreq);		
		Time::HPCFreqF = (f64)hpcfreq.QuadPart;
		Time::HPCFreq = hpcfreq.QuadPart;
	}
} _TimeInitator;
}	}



LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )	{ 
	auto &ui = JUI::JUI_Main::ptr();
	switch( msg ) {
		//*
		case WM_KEYDOWN		: if(ui.keyDown((u8)wParam))		return 0; break;
		case WM_LBUTTONDOWN : if(ui.keyDown(VK_LBUTTON))	return 0; break;
		case WM_RBUTTONDOWN : if(ui.keyDown(VK_RBUTTON))	return 0; break;
		case WM_MBUTTONDOWN : if(ui.keyDown(VK_MBUTTON))	return 0; break;
		case WM_KEYUP		: if(ui.keyUp((u8)wParam))			return 0; break;
		case WM_LBUTTONUP	: if(ui.keyUp(VK_LBUTTON))		return 0; break;
		case WM_RBUTTONUP	: if(ui.keyUp(VK_RBUTTON))		return 0; break;
		case WM_MBUTTONUP	: if(ui.keyUp(VK_MBUTTON))		return 0; break;
		
		case WM_MOUSEMOVE	:	if(ui.mMove(vec2s16( s16( LOWORD( lParam ) ), s16( HIWORD( lParam ) ))))		return 0; break;

			/*	
		case WM_MOUSEWHEEL:		MPos.z +=(s16)HIWORD(wParam)/120; break;
		case WM_KEYUP:			if( InputMan.keyUp(wParam) ) return 0;	break; 
		case WM_KEYDOWN: {
			if( UIMan.FocusData.Type == cFocusData::FD_TextLine && UIMan.FocusData.TL.TC->Key(wParam) ) return 0;
			if( InputMan.keyDown(wParam) )		return 0;	break;
		}
		case WM_CHAR:			
			Assert( UIMan.FocusData.Type == cFocusData::FD_TextLine, "WM_CHAR for??" );
			PRINT("WM_CHAR "<<(char)wParam<<"  -- "<<(u32)wParam<<"\n");  
			Assert( wParam < 256, "uh oh ");
			UIMan.Focus->Char( wParam );
			return 0;*/
		case WM_DESTROY: // window destroy
		case WM_CLOSE : 
			PostQuitMessage(0); //Send a WM_QUIT msg
			return 0;
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps); 
			EndPaint(hWnd, &ps);
			return 0;
		}
		case WM_SYSCOMMAND:
			// prevent screensaver or monitor powersave mode from starting
			if ((wParam & 0xFFF0) == SC_SCREENSAVE ||
				(wParam & 0xFFF0) == SC_MONITORPOWER)
				return 0;
			break;
		case WM_USER: return 0;
		case WM_ERASEBKGND: return 0;  

	}
	return DefWindowProc(hWnd,msg,wParam,lParam); 
}

void checkError() {
	DWORD errorCode=GetLastError();
	if( errorCode ) {
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf, 0, NULL);
		MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
		LocalFree(lpMsgBuf);
		SetLastError( 0);
		DebugBreak();
	}
}

void clearError() {
	SetLastError( 0);
}

#ifndef UTIL_BUILD
s32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, s32 showCmd) {	
	 

	DuctTape::activateForModule();
#ifdef DEBUG
 	WinG::initConsole();
#endif 
	//CreateNamedPipe()
	{	

		HCURSOR Cursors[16];
		Cursors[0] = LoadCursor(NULL, IDC_ARROW); 
		Cursors[1] = LoadCursor(NULL, IDC_IBEAM);    
		Cursors[2] = LoadCursor(NULL, IDC_WAIT);    
		Cursors[3] = LoadCursor(NULL, IDC_CROSS);   
		Cursors[4] = LoadCursor(NULL, IDC_UPARROW);   
		Cursors[5] = LoadCursor(NULL, IDC_SIZE);   
		Cursors[6] = LoadCursor(NULL, IDC_ICON);   
		Cursors[7] = LoadCursor(NULL, IDC_SIZENWSE);   
		Cursors[8] = LoadCursor(NULL, IDC_SIZENESW);    
		Cursors[9] = LoadCursor(NULL, IDC_SIZEWE);    
		Cursors[10] = LoadCursor(NULL, IDC_SIZENS);    
		Cursors[11] = LoadCursor(NULL, IDC_SIZEALL);    
		Cursors[12] = LoadCursor(NULL, IDC_NO);    
		Cursors[13] = LoadCursor(NULL, IDC_HAND);    
		Cursors[14] = LoadCursor(NULL, IDC_APPSTARTING);    
		Cursors[15] = LoadCursor(NULL, IDC_HELP);    

		SetLastError( 0); //todo .. some cursor not loading in windows 8 ...do't really care at this point
		
		// Register class
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof( WNDCLASSEX );
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wcex.lpfnWndProc = WndProc; 
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon =LoadIcon(NULL, IDI_APPLICATION);//LoadIcon( hInstance, MAKEINTRESOURCE(101) );
		wcex.hCursor = Cursors[11]; 
		wcex.hbrBackground = null;// (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = "AnAwesomeWindowClassName";
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


		//if( !RegisterClassEx( &wcex )) ThrowCrit("Failed to RegisterClass");
		RegisterClassEx( &wcex );


		checkError();
		Gem_Main::ScopeHandle hndl;

		MSG msg = {0};
		//Time time;
		for(;;) {
			auto &ui = JUI::JUI_Main::ptr();
			while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )  {
				TranslateMessage( &msg );
				DispatchMessage( &msg ); 
				if (msg.message == WM_QUIT)
					ExitProcess(-1); // not finished proper cleanup
			}
			ui.update();

		//	std::cout<<" time "<<time.updateElap().asI()<<"\n";
			Sleep(1 );			
		}
		
		hndl.shutdown();
	//	Gem_Main::shutdown();
	}

	DuctTape::printAll();

	return 0;
}
#endif


f64 Time::HPCFreqF;
s64 Time::HPCFreq;

s64 Time::Current() {
	LARGE_INTEGER temp;
	QueryPerformanceCounter(&temp);
	return temp.QuadPart;
}

namespace Gem { 
void debugBreak() {
	DebugBreak();
}


namespace _Internal { 
	 
void _INT_forceLink_OS() {}

}
}   


