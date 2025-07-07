#include "pch.h"
#include "TriangleDemo.h"
#include "Main/ClientActor.h"
#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Texture.h"
#include "Graphics/Shader/ShaderInfos.h"

void TriangleDemo::BeginPlay()
{
	_clientActor = make_shared<ClientActor>();
	_clientActor->Construct();

	StaticMeshInfo meshInfo;
	meshInfo.mesh = make_shared<Mesh>();
	meshInfo.texture = make_shared<Texture>();
	meshInfo.mesh->CreateQuad();
	meshInfo.texture->Load(L"..\\Resources\\Images\\Guitarist.png");
	meshInfo.shaderInfos = make_shared<ShaderInfos>(L"00. Triangle.hlsl");

	_clientActor->SetStaticMeshInfo(meshInfo);

	_clientActor->BeginPlay();
}

void TriangleDemo::Tick()
{
	_clientActor->Tick();
}

void TriangleDemo::Render()
{
	_clientActor->Render();
}
