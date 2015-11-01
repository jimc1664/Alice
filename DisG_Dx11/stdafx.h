
#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN 
#include <windows.h>


#include "d3d11_1.h"
#include "D3D11.h"
//#include "D3DX11.h"

#undef DrawText

#pragma comment( lib, "d3d11.lib" )

#ifdef DEBUG
#pragma comment( lib, "d3dx11d.lib" )
#else
#pragma comment( lib, "d3dx11.lib" )
#endif

#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "D3Dcompiler.lib" )
#pragma comment( lib, "dxguid.lib" )

#include "../Gem/Common_Internal.h"

namespace Gem { namespace Dx11 {} namespace Dis {} }
using namespace Dx11; //using namespace Dis;
