#include "pch.h"
#include "LoadMaterialDemo.h"
#include "Main/ClientPawn.h"
#include "Resource/Mesh.h"
#include "Resource/Texture.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Actor/CameraActor.h"
#include "Resource/Material.h"
#include "Resource/StaticMesh.h"
#include "Components/StaticMeshRenderer.h"
#include "Components/Transform.h"

void LoadMaterialDemo::Construct()
{
	// 이쪽에서 정해줄 수 있게 하는 것들은 나중에 UI로 빼기 쉬움

	// Resource Load
	RESOURCE_MANAGER->BeginPlay();
	//RESOURCE_MANAGER->Load<Texture>(L"Guitarist", L"..\\Resources\\Images\\Guitarist2.png");
	RESOURCE_MANAGER->LoadStaticMeshFromXML(L"Tank", L"Tank");

	_clientPawn = make_shared<ClientPawn>();
	_clientPawn->Construct();
	_clientPawn->_actorName = "ClientPawn";
	
	shared_ptr<StaticMesh> staticMesh = RESOURCE_MANAGER->Get<StaticMesh>(L"Tank");
	_clientPawn->SetStaticMesh(staticMesh);
	_clientPawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 0.f, 20.f });
	//_clientPawn->GetOrAddTransform()->SetWorldScale(Vec3(1.f));
	//_clientPawn->GetOrAddTransform()->SetLocalRotation({ 0.0f, 0.0f, 0.0f });

	shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"03. LightTest.hlsl");
	_clientPawn->GetOrAddStaticMeshRenderer()->SetShaderInfo(shaderInfo);

	// Scene에 등장
	SCENE->Add(_clientPawn);
}

void LoadMaterialDemo::BeginPlay()
{
	SHADER_PARAM_MANAGER->BeginPlay();
	_clientPawn->BeginPlay();
}

void LoadMaterialDemo::Tick()
{
	

	_clientPawn->Tick();
}

void LoadMaterialDemo::Render()
{
	
}
