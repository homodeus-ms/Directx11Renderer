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
#include <unordered_set>
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

// Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

// ImGui
#include "Utils/ImGui/imgui.h"
#include "Utils/ImGui/imgui_impl_dx11.h"
#include "Utils/ImGui/imgui_impl_win32.h"

// Libs
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug.lib")
#pragma comment(lib, "Assimp/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "DirectXTex/DirectXTex.lib")
#pragma comment(lib, "Assimp/assimp-vc143-mt.lib")
#endif

#pragma comment(lib, "C:/vcpkg/installed/x64-windows/lib/DirectXTK.lib")

// Managers
#include "Engine/Renderer.h"
#include "Engine/Graphics.h"
#include "Managers/InputManager.h"
#include "Managers/TimeManager.h"
#include "Managers/ResourceManager.h"
#include "Utils/ImGui/ImGuiManager.h"
#include "Managers/NameManager.h"



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
