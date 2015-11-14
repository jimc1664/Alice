#include "stdafx.h"

#include "RenderTarget.h"
#include "Dx11_Task.h"



Dis::RenderTarget::RenderTarget( const vec2u16 &size, OS::Window &wndw  ) : Wndw( wndw), Size(size) {

	struct Init_RT : public Dx11_Task<Init_RT> {
		Init_RT( Dx11::RenderTarget *rt ) : RT(*rt) {}
		void proc( Dx11_Main &dm ) { RT.init(dm ); }
		Dx11::RenderTarget &RT;
	};
	new Init_RT( static_cast<Dx11::RenderTarget*>(this) );
}

void RenderTarget::init( Dx11_Main &dm ) {	
	static_assert(sizeof( *this ) == sizeof(Dis::RenderTarget ), "Dx11::RenderTarget - size mismatch" );

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
	GThrowOnFAILED( gm.Device->CreateRenderTargetView( tTex, NULL, &RTV ),						"Failed to create Main RT" );
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
	gm.Device->ClearRenderTargetView( RTV, c);

	//if( CurTarget == &MainTarget ) set( *this );
	if( gm.CurTarget == this ) set( gm );
	Rebuild = 0; */
}



