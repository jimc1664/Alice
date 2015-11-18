#include "stdafx.h"

#include "Gem/Dis/MainTarget.h"
#include "OGL_Task.h"
#include "Gem/MainWindow.h"
#include "Gem/Dis/DrawList.h"



void checkError();

MainTarget::MainTarget( const vec2u16 &size, OS::Window &wndw ) : Wndw(wndw), RenderTarget(size) {
/*
	struct Init_MainTarget : public OGL_Task<Init_MainTarget> {
		void proc( DisMain &dm ) { RT.init(dm ); }
		MainTarget &RT;
	private: friend class OGL_Task<Init_MainTarget>;
		Init_MainTarget( MainTarget *rt ) : RT(*rt) {}
	};
	Init_MainTarget::ctor( static_cast<MainTarget*>(this) );

	*/

	Singleton<DisMain>::ptr().Primary = this;  //todo....
}

//typedef HGLRC(APIENTRYP PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);

void MainTarget::init( DisMain &dm ) {	
	//static_assert(sizeof( *this ) == sizeof(MainTarget ), "MainTarget - size mismatch" );

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	
	wglMakeCurrent(NULL, NULL);
	Hdc = GetDC( Wndw.hndl() );

	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size
		1,                          // version
		PFD_SUPPORT_OPENGL |        // OpenGL window
		PFD_DRAW_TO_WINDOW |        // render to window
		PFD_DOUBLEBUFFER,           // support double-buffering
		PFD_TYPE_RGBA,              // color type
		32,                         // prefered color depth
		0, 0, 0, 0, 0, 0,           // color bits (ignored)
		0,                          // no alpha buffer
		0,                          // alpha bits (ignored)
		0,                          // no accumulation buffer
		0, 0, 0, 0,                 // accum bits (ignored)
		16,                         // depth buffer
		0,                          // no stencil buffer
		0,                          // no auxiliary buffers
		PFD_MAIN_PLANE,             // main layer
		0,                          // reserved
		0, 0, 0,                    // no layer, visible, damage masks
	};

	pixelFormat = ChoosePixelFormat(Hdc, &pfd);
	SetPixelFormat(Hdc, pixelFormat, &pfd);

	Cntx = wglCreateContext(Hdc);
	wglMakeCurrent(Hdc, Cntx);
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	//If this is NULL then OpenGL 3.0 is not supported

	if (!wglCreateContextAttribsARB) {
		Error("OpenGL 3.0 is not supported, falling back to GL 2.1");
		//pInstance->m_hglrc = tmpContext;
	} else {
		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
			0 //zero indicates the end of the array
		}; 

		HGLRC nCntxt = wglCreateContextAttribsARB(Hdc, 0, attribs);
		if( nCntxt ) {
			wglDeleteContext(Cntx);
			wglMakeCurrent(Hdc, nCntxt);
			Cntx = nCntxt;
		}
	}
	checkError();
	
	if( GLEW_OK != glewInit() ) {
		int err = glGetError();
		auto es = glewGetErrorString( err );
	}
	else {
		/* flush the OpenGL error state, ignoring all errors */
		while( GL_NO_ERROR != glGetError() );
	}

	int err2 = GetLastError();
	SetLastError(0);
	dm.Outputs.add(this);
	dm.Cntx = Cntx;





	/*
	 DWORD errorCode=GetLastError();
     LPVOID lpMsgBuf;
     FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf, 0, NULL);
     MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
     LocalFree(lpMsgBuf);
	 */
	/*Vp.TopLeftX = Vp.TopLeftY = 0;
	Vp.MaxDepth = 1;
	Vp.MinDepth = 0;
	Vp.Width = ss.x;
	Vp.Height = ss.y;

	Size = (vec2s)ss;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 2;
	sd.BufferDesc.Width = ss.x;
	sd.BufferDesc.Height = ss.y;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = Wndw.Hndl;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect =DXGI_SWAP_EFFECT_DISCARD;

	GThrowOnFAILED( gm.DXFactory->CreateSwapChain( gm.Device, &sd, &SwapChain),						"Failed to create swap chain");
		// Create a render target view
	ID3D10Texture2D* tTex;
	GThrowOnFAILED( SwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&tTex ),	"Failed to create Main RT tTex" );
	GThrowOnFAILED( gm.Device->CreateMainTargetView( tTex, NULL, &RTV ),						"Failed to create Main RT" );
	tTex->Release();

	D3D10_TEXTURE2D_DESC texDesc;
	texDesc.Width				= ss.x;				texDesc.Height				= ss.y;
	texDesc.MipLevels			= 1;				texDesc.ArySize			= 1;
	texDesc.SampleDesc.Count	= 1;				texDesc.SampleDesc.Quality	= 0;
	texDesc.CPUAccessFlags		= 0;				texDesc.MiscFlags			= 0;
	texDesc.Usage				= D3D10_USAGE_DEFAULT;
	texDesc.BindFlags			= D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	texDesc.Format				= DXGI_FORMAT_R32_TYPELESS;

	GThrowOnFAILED( gm.Device->CreateTexture2D( &texDesc, NULL, &tTex ),							"Failed to create Depth Tex" );

	D3D10_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension		= D3D10_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice	= 0;
	GThrowOnFAILED( gm.Device->CreateDepthStencilView( tTex, &dsvDesc, &RTD),					"Failed to create RTDMain" );

	f32 c[] = { 0,0,0.4,0, };
	gm.Device->ClearMainTargetView( RTV, c);

	//if( CurTarget == &MainTarget ) set( *this );
	if( gm.CurTarget == this ) set( gm );
	Rebuild = 0; */
}

void MainTarget::render(RenderingCntx &rc) {

	RenderTarget::render(rc);


	glClearColor(0.1f, 0.1f, 0.14f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawL.render(rc);

	SwapBuffers(Hdc);
}