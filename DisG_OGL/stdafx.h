
#pragma once

#define DIS_INTERNAL

#include <SDKDDKVer.h>
//*

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define VC_EXTRALEAN 
#include <windows.h>

#define GLEW_STATIC
//#include <gl/GL.h>
#include "GL/glew.h"
#include "GL/wglext.h"

#include "../Gem/Common_Internal.h"

#pragma comment (lib, ExtLibName( "glew32s" ) ) 
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "gdi32.lib")    /* link Windows GDI lib        */
#pragma comment (lib, "winmm.lib")    /* link Windows MultiMedia lib */
#pragma comment (lib, "user32.lib")   /* link Windows user lib       */
/*
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
*/

namespace Gem { namespace Dis {} }
//using namespace OGL; //using namespace Dis;



#include "OGlMain.h"

using namespace Dis;

