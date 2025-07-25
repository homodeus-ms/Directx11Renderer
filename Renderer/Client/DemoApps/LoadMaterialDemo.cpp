#include "pch.h"
#include "LoadMaterialDemo.h"
#include "Main/ClientPawn.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/BasicMesh/DefaultBasicMesh.h"
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


LoadMaterialDemo::~LoadMaterialDemo()
{
}

void LoadMaterialDemo::Construct()
{
	// 이쪽에서 정해줄 수 있게 하는 것들은 나중에 UI로 빼기 쉬움
	
	// GUI Controller -> TODO : 공통 시작 코드 부분 모으기
	//FLOW_MANAGER->BeginPlay();
	g_FlowManager->BeginPlay();

	// Resource Load
	//RESOURCE_MANAGER->Load<Texture>(L"Guitarist", L"..\\Resources\\Images\\Guitarist2.png");
	RESOURCE_MANAGER->LoadStaticMeshFromXML(L"Tank", L"Tank");
	RESOURCE_MANAGER->LoadStaticMeshFromXML(L"Zelda", L"Zelda");
	RESOURCE_MANAGER->Load<Texture>(L"Leather", L"..\\Resources\\Images\\Leather.jpg");
	RESOURCE_MANAGER->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Images\\Leather_Normal.jpg");
	// C:\\Users\\seekc\\OneDrive\\Documents\\Renderer\\Renderer
	RESOURCE_MANAGER->Load<Texture>(L"Snow", L"..\\Resources\\Images\\DDS\\Areskutan.dds");
	RESOURCE_MANAGER->Load<Texture>(L"CubeMap1", L"..\\Resources\\Images\\DDS\\SanFrancisco2.dds");
	RESOURCE_MANAGER->Load<Texture>(L"CubeMap2", L"..\\Resources\\Images\\DDS\\SanFrancisco4.dds");
	RESOURCE_MANAGER->Load<Texture>(L"CubeMap3", L"..\\Resources\\Images\\DDS\\DropdownTriangle.dds");
	RESOURCE_MANAGER->Load<Texture>(L"CubeMap4", L"..\\Resources\\Images\\DDS\\Lycksele3.dds");
	RESOURCE_MANAGER->Load<Texture>(L"Night", L"..\\Resources\\Images\\DDS\\NightPath.dds");
	RESOURCE_MANAGER->Load<Texture>(L"Skybox", L"..\\Resources\\Images\\DDS\\Skybox.dds");

	// Material
	shared_ptr<Material> material = make_shared<Material>();
	material->SetDiffuseMap(RESOURCE_MANAGER->Get<Texture>(L"Leather"));
	material->SetNormalMap(RESOURCE_MANAGER->Get<Texture>(L"Leather_Normal"));
	{
		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(0.5f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		desc.emissive = Vec4(1.f, 0.f, 0.0f, 1.f);
		desc.bUnLit = 0;
		desc.MaterialType = EMaterialType::RimLight;
	}
	shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
	material->SetShaderInfo(shaderInfo);
	RESOURCE_MANAGER->Add(L"Leather", material);
	
	// Client
	if (0)
	{
		shared_ptr<ClientPawn> clientPawn = make_shared<ClientPawn>("Tank");
		shared_ptr<StaticMesh> staticMesh = RESOURCE_MANAGER->Get<StaticMesh>(L"Tank");

		clientPawn->SetStaticMesh(staticMesh);
		//_clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(0.03f));
		clientPawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 0.f, 0.f });
		clientPawn->GetOrAddTransform()->SetWorldRotation({ -0.1f, 0.0f, 0.0f });

		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicShader.hlsl");
		clientPawn->GetOrAddStaticMeshRenderer()->SetShaderInfo(shaderInfo);
		SCENE->AddActor(clientPawn);
	}

	if (0)
	{
		shared_ptr<ClientPawn> clientPawn = make_shared<ClientPawn>("Zelda");

		shared_ptr<StaticMesh> staticMesh = RESOURCE_MANAGER->Get<StaticMesh>(L"Zelda");
		
		clientPawn->SetStaticMesh(staticMesh);
		clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(0.03f));
		clientPawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 0.f, 0.f });
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicShader.hlsl");
		clientPawn->GetOrAddStaticMeshRenderer()->SetShaderInfo(shaderInfo);
		SCENE->AddActor(clientPawn);
	}

	// CubeMap
	if (0)
	{
		shared_ptr<Material> cubeMapMat = make_shared<Material>();
		shared_ptr<Texture> texture = RESOURCE_MANAGER->Get<Texture>(L"Skybox");
		cubeMapMat->SetDiffuseMap(texture);
		{
			MaterialDesc& desc = cubeMapMat->GetMaterialDesc();
			desc.ambient = Vec4(1.f);
			desc.diffuse = Vec4(1.f);
			desc.specular = Vec4(1.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
		}
		shared_ptr<ShaderInfo> cubeMapShader = make_shared<ShaderInfo>(L"CubeMapShader.hlsl");
		cubeMapMat->SetShaderInfo(cubeMapShader);
		RESOURCE_MANAGER->Add(L"CubeMap", cubeMapMat);

		shared_ptr<Actor> pawn = make_shared<ClientPawn>();
		pawn->Construct();
		pawn->GetTransform()->SetWorldPosition({ 0.f, 0.f, 0.f });

		shared_ptr<BasicMesh> mesh;
		mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"CubeMap");

		pawn->SetBasicMesh(mesh);
		pawn->SetBasicMaterial(cubeMapMat);

		SCENE->AddActor(pawn);
	}


	if (1)
	{
		if (0) // 바닥 평면
		{
			shared_ptr<Actor> pawn = make_shared<ClientPawn>();
			pawn->Construct();

			pawn->GetOrAddTransform()->SetLocalScale({ 80.f, 80.f, 80.f });
			pawn->GetOrAddTransform()->SetLocalRotation({ 3.14f / 2, 0.0f, 0.f });
			pawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 0.f, 0.f });

			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Quad");

			shared_ptr<Material> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite");
			pawn->SetBasicMesh(mesh);
			pawn->SetBasicMaterial(mat);
			pawn->SetIsCastShadowedActor(false);

			SCENE->AddActor(pawn);
		}
		
		if (1) // Room
		{
			shared_ptr<Actor> pawn = make_shared<ClientPawn>();
			pawn->Construct();
			
			pawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 0.f, 0.f });

			shared_ptr<BasicMesh> mesh = make_shared<DefaultBasicMesh>();
			mesh->CreateSquareRoom();

			shared_ptr<Material> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite");
			pawn->SetBasicMesh(mesh);
			pawn->SetBasicMaterial(mat);
			pawn->SetIsCastShadowedActor(false);

			SCENE->AddActor(pawn);
		}

		// 큐브
		if (1)
		{
			shared_ptr<Actor> pawn = make_shared<ClientPawn>();
			pawn->Construct();

			pawn->GetOrAddTransform()->SetLocalScale({ 2.f, 2.f, 2.f });
			pawn->GetTransform()->SetLocalRotation({ 0.f, 3.14f / 4, 0.f });
			pawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 2.f, -5.f });

			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Cube");
			shared_ptr<Material> mat = RESOURCE_MANAGER->Get<Material>(L"BasicRed");
			pawn->SetBasicMesh(mesh);
			pawn->SetBasicMaterial(mat);

			SCENE->AddActor(pawn);
		}

		// 큰 구
		if (1)
		{
			shared_ptr<Actor> pawn2 = make_shared<ClientPawn>();
			pawn2->GetOrAddTransform()->SetLocalScale({ 5.f, 5.f, 5.f });
			pawn2->GetOrAddTransform()->SetWorldPosition({ 0.f, 3.5f, 10.f });

			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Sphere");
			shared_ptr<Material> mat = RESOURCE_MANAGER->Get<Material>(L"BasicYellow");
			pawn2->SetBasicMesh(mesh);
			pawn2->SetBasicMaterial(mat);

			SCENE->AddActor(pawn2);
		}

		if (1)
		{
			shared_ptr<Actor> pawn2 = make_shared<ClientPawn>();
			pawn2->GetOrAddTransform()->SetLocalScale({ 3.f, 3.f, 3.f });
			pawn2->GetOrAddTransform()->SetWorldPosition({ 2.f, 2.5f, 0.f });

			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Sphere");
			shared_ptr<Material> mat = RESOURCE_MANAGER->Get<Material>(L"BasicGreen");
			pawn2->SetBasicMesh(mesh);
			pawn2->SetBasicMaterial(mat);

			SCENE->AddActor(pawn2);
		}
	}

	// IBL
	//SCENE->CreateEnvironment(L"Night");
	//SCENE->SetEnvLightTexture(L"Night");
	
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
