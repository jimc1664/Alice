
#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN 
#include <windows.h>


#include "d3d10_1.h"
#include "D3D10.h"
//#include "D3DX10.h"

#undef DrawText

#pragma comment( lib, "d3d10.lib" )

#ifdef DEBUG
#pragma comment( lib, "d3dx10d.lib" )
#else
#pragma comment( lib, "d3dx10.lib" )
#endif

#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "D3Dcompiler.lib" )
#pragma comment( lib, "dxguid.lib" )

#include "../Gem/Common_Internal.h"

namespace Gem { namespace Dx10 {} namespace Dis {} }
using namespace Dx10; //using namespace Dis;
