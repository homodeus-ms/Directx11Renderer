#include "pch.h"
#include "CameraDemo.h"
#include "Main/ClientPawn.h"
#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Texture.h"
#include "Graphics/Shader/ShaderInfos.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Actor/CameraActor.h"

void CameraDemo::Construct()
{
	// Create Actor
	_clientPawn = make_shared<ClientPawn>();
	_clientPawn->Construct();
	_clientPawn->_actorName = "ClientPawn";

	// Attach Camera
	_cameraActor = make_shared<CameraActor>(_clientPawn);
	_cameraActor->Construct();
	_cameraActor->_actorName = "CameraActor";

	_clientPawn->AttachFollowCamera(_cameraActor);

	// Create and Set Mesh Info
	StaticMeshInfo meshInfo;
	meshInfo.mesh = make_shared<Mesh>();
	meshInfo.texture = make_shared<Texture>();
	meshInfo.mesh->CreateQuad();
	meshInfo.texture->Load(L"..\\Resources\\Images\\Guitarist.png");
	meshInfo.shaderInfos = make_shared<ShaderInfos>(L"01. CameraTest.hlsl");

	_clientPawn->SetStaticMeshInfo(meshInfo);
}

void CameraDemo::BeginPlay()
{
	_clientPawn->BeginPlay();
}

void CameraDemo::Tick()
{
	_clientPawn->Tick();
}

void CameraDemo::Render()
{
	_clientPawn->Render();
}
