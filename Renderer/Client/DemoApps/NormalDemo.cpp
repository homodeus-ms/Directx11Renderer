#include "pch.h"
#include "NormalDemo.h"
#include "Main/ClientPawn.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/Texture.h"
#include "Components/Transform.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Actor/CameraActor.h"
#include "Resource/Material.h"
#include "UI/Controller/GUIController.h"
#include "Actor/LightActor.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/DirectionalLight.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"

void NormalDemo::Construct()
{
	// 이쪽에서 정해줄 수 있게 하는 것들은 나중에 UI로 빼기 쉬움
	_guiController = make_shared<GUIController>();
	_guiController->BeginPlay();

	// Resource Load
	RESOURCE_MANAGER->Load<Texture>(L"Leather", L"..\\Resources\\Images\\Leather.jpg");
	RESOURCE_MANAGER->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Images\\Leather_Normal.jpg");

	// Material
	shared_ptr<Material> material = make_shared<Material>();
	material->SetDiffuseMap(RESOURCE_MANAGER->Get<Texture>(L"Leather"));
	material->SetNormalMap(RESOURCE_MANAGER->Get<Texture>(L"Leather_Normal"));
	{
		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(0.5f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		
	}
	shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
	material->SetShaderInfo(shaderInfo);
	RESOURCE_MANAGER->Add(L"Leather", material);

	// Client Pawn
	{
		shared_ptr<Actor> pawn = make_shared<ClientPawn>();
		pawn->Construct();
		
		pawn->GetOrAddTransform()->SetWorldPosition({ -1.f, 0.f, 1.f });

		shared_ptr<BasicMesh> mesh;
		mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Cube");

		pawn->SetBasicMesh(mesh);
		pawn->SetBasicMaterial(material);

		SCENE->AddActor(pawn);
	}

	{
		_clientPawn = make_shared<ClientPawn>();
		
		_clientPawn->Construct();
		_clientPawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 0.f, 0.f });

		shared_ptr<BasicMesh> mesh;
		mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Sphere");

		_clientPawn->SetBasicMesh(mesh);
		_clientPawn->SetBasicMaterial(material);
		
		SCENE->AddActor(_clientPawn);
	}

	_spotLight1 = SCENE->AddSpotLight();
	

}

void NormalDemo::BeginPlay()
{
	//SCENE->TurnGlobalLightOnOff(false);
}

void NormalDemo::Tick()
{
	if (1)
	{
		shared_ptr<LightActor> global = SCENE->GetGlobalLight();
		DirectionalLightDesc* getDesc = static_cast<DirectionalLightDesc*>(global->GetDesc());
		
		getDesc->ambient = { 0.6f, 0.6f, 0.6f, 1.f };
		//getDesc->ambient = { 0.2f, 0.2f, 0.2f, 1.f };
		getDesc->diffuse = { 1.0f, 1.0f, 1.0f, 1.f };
		getDesc->specular = { 1.f, 1.f, 1.f, 1.f };
		getDesc->direction = { 0.f, 0.f, 1.f };
	}

	SpotLightDesc* desc = static_cast<SpotLightDesc*>(_spotLight1->GetDesc());
	desc->ambient = { 0.1f, 0.1f, 0.1f, 1.f };
	desc->diffuse = { 1.f, 1.f, 1.f, 1.f };
	desc->specular = { 1.f, 1.f, 1.f, 1.f };
	desc->emissive = { 0.f, 0.f, 0.f, 1.f };
	desc->direction = { 0.f, -1.f, 0.f };
	desc->position = { 0.f, 5.f, 0.f };
	desc->range = 20;
	desc->spotPower = 5;
}

void NormalDemo::Render()
{
	
}
