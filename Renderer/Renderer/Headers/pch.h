#pragma once

#define WIN32_LEAN_AND_MEAN 

#include "Types.h"
#include "Define.h"
#include "Utils/Log.h"

// STL
#include <memory>
#include <iostream>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;

// WIN
#include <windows.h>
#include <assert.h>
#include <optional>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>
using namespace DirectX;
using namespace Microsoft::WRL;

#include <FX11/d3dx11effect.h>

// Libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#pragma comment(lib, "FX11/Effects11d.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#pragma comment(lib, "FX11/Effects11.lib")
#endif

// Managers
#include "Managers/Renderer.h"
#include "Managers/Graphics.h"
#include "Managers/InputManager.h"
#include "Managers/TimeManager.h"
#include "Managers/ResourceManager.h"

#define check(p)			assert(SUCCEEDED(p))
#define RENDERER			GET_SINGLE(Renderer)	
#define INPUT_MANAGER		GET_SINGLE(InputManager)
#define TIME_MANAGER		GET_SINGLE(TimeManager)
#define RESOURCE_MANAGER	GET_SINGLE(ResourceManager)
#define GRAPHICS			GET_SINGLE(Graphics)

#define DEVICE				GRAPHICS->GetDevice()
#define CONTEXT				GRAPHICS->GetDeviceContext()
#define DT					TIME_MANAGER->GetDeltaTime()


// Engine
//#include "VertexData.h"
//#include "VertexBuffer.h"
//#include "IndexBuffer.h"
//#include "ConstantBuffer.h"
//#include "Shader.h"
//#include "IExecute.h"
//
//#include "GameObject.h"
//#include "Transform.h"
//#include "Texture.h"
//#include "Mesh.h"
