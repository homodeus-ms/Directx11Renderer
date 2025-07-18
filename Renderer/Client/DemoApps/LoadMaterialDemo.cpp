#include "pch.h"
#include "LoadMaterialDemo.h"
#include "Main/ClientPawn.h"
#include "Resource/BasicMesh.h"
#include "Resource/Texture.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Actor/CameraActor.h"
#include "Resource/Material.h"
#include "Resource/StaticMesh.h"
#include "Components/StaticMeshRenderer.h"
#include "Components/Transform.h"
#include "Actor/LightActor.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/DirectionalLight.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"
#include "Actor/BulbActor.h"


void LoadMaterialDemo::Construct()
{
	// 이쪽에서 정해줄 수 있게 하는 것들은 나중에 UI로 빼기 쉬움
	
	// GUI Controller -> TODO : 공통 시작 코드 부분 모으기
	FLOW_MANAGER->BeginPlay();

	// Resource Load
	//RESOURCE_MANAGER->Load<Texture>(L"Guitarist", L"..\\Resources\\Images\\Guitarist2.png");
	RESOURCE_MANAGER->LoadStaticMeshFromXML(L"Tank", L"Tank");
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
	
	// Client
	if (0)
	{
		_clientPawn = make_shared<ClientPawn>();
		_clientPawn->Construct();

		shared_ptr<StaticMesh> staticMesh = RESOURCE_MANAGER->Get<StaticMesh>(L"Tank");
		_clientPawn->SetStaticMesh(staticMesh);
		//_clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(0.03f));
		_clientPawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 0.f, 0.f });
		_clientPawn->GetOrAddTransform()->SetWorldRotation({ -0.1f, 0.0f, 0.0f });

		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"03. LightTest.hlsl");
		_clientPawn->GetOrAddStaticMeshRenderer()->SetShaderInfo(shaderInfo);
		SCENE->AddActor(_clientPawn);
	}

	if (1)
	{
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
	}

	// 전구
	//_bulb = make_shared<BulbActor>();
	//_bulb->Construct();
	//SCENE->Add(_bulb);
}

void LoadMaterialDemo::BeginPlay()
{
	//SHADER_PARAM_MANAGER->BeginPlay();
	//_clientPawn->BeginPlay();

	//SCENE->TurnGlobalLightOnOff(false);
}

void LoadMaterialDemo::Tick()
{
	/*Vec3 pos = _clientPawn->GetTransform()->GetWorldPosition();
	pos.x += 1.f * TIME_MANAGER->GetDeltaTime();
	_clientPawn->GetTransform()->SetWorldPosition(pos);*/

	//_clientPawn->Tick();

	/*SpotLightDesc* desc = static_cast<SpotLightDesc*>(_spotLight1->GetDesc());
	desc->ambient = { 0.1f, 0.1f, 0.1f, 1.f };
	desc->diffuse = { 1.9f, 1.9f, 1.9f, 1.f };
	desc->specular = { 1.f, 1.f, 1.f, 1.f };
	desc->emissive = { 0.f, 0.f, 0.f, 1.f };
	desc->direction = { 0.f, 0.f, 1.f };
	desc->position = { 0.f, 0.f, -5.f };
	desc->range = 100;
	desc->spotPower = 1;*/
	
}

void LoadMaterialDemo::Render()
{
	
}
