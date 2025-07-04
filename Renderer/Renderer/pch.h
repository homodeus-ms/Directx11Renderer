#pragma once

#define _XM_NO_INTRINSICS_

#include "Types.h"
#include "Values.h"
#include "Log.h"
#include "Struct.h"
#include "Defines.h"

// STL
#include <memory>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
using namespace std;

// WIN
#include <windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;  // Comptr

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

#define check(p)                            \
{                                           \
    if (!SUCCEEDED(p))                      \
    {                                       \
        LOG(Error, "NOT SUCCEEDED!");       \
        assert(false);                      \
    }                                       \
}                      

#include "02. Graphics/Graphics.h"
#include "02. Graphics/Pipeline/00. Geometry/Geometry.h"