#include "pch.h"
#include "LoadMaterialDemo.h"
#include "Main/ClientPawn.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/BasicMesh/DefaultBasicMesh.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Resource/LUT.h"
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
	RESOURCE_MANAGER->LoadMeshFromAssetFolder(L"Tank", L"Tank");
	RESOURCE_MANAGER->LoadMeshFromAssetFolder(L"Zelda", L"Zelda");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Leather", L"..\\Resources\\Images\\Leather.jpg");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Leather_Normal", L"..\\Resources\\Images\\Leather_Normal.jpg");
	// C:\\Users\\seekc\\OneDrive\\Documents\\Renderer\\Renderer
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"Snow_spec", L"..\\Resources\\Images\\DDS\\Areskutan.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"Snow_diff", L"..\\Resources\\Images\\DDS\\Areskutan.dds");

	
	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightCity1_spec", L"..\\Resources\\Images\\DDS\\NightCity1\\NightCity1_spec.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightCity1_diff", L"..\\Resources\\Images\\DDS\\NightCity1\\NightCity1_diff.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightCity2_spec", L"..\\Resources\\Images\\DDS\\NightCity2\\NightCity2_spec.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightCity2_diff", L"..\\Resources\\Images\\DDS\\NightCity2\\NightCity2_diff.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightPath_spec", L"..\\Resources\\Images\\DDS\\NightPath\\NightPath_spec.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightPath_diff", L"..\\Resources\\Images\\DDS\\NightPath\\NightPath_diff.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Museum_spec", L"..\\Resources\\Images\\DDS\\Museum\\Museum_spec.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Museum_diff", L"..\\Resources\\Images\\DDS\\Museum\\Museum_diff.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"SnowForest_spec", L"..\\Resources\\Images\\DDS\\SnowForest\\SnowForest_spec.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"SnowForest_diff", L"..\\Resources\\Images\\DDS\\SnowForest\\SnowForest_diff.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"DawnField_spec", L"..\\Resources\\Images\\DDS\\DawnField\\DawnField_spec.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"DawnField_diff", L"..\\Resources\\Images\\DDS\\DawnField\\DawnField_diff.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"DayLight1_spec", L"..\\Resources\\Images\\DDS\\DayLight1\\DayLight1_spec.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"DayLight1_diff", L"..\\Resources\\Images\\DDS\\DayLight1\\DayLight1_diff.dds");

	RESOURCE_MANAGER->Load<LUT>(L"LUT_BlueArchitecture", L"..\\Resources\\LUT\\BlueArchitecture.cube");
	//RESOURCE_MANAGER->Load<LUT>(L"LUT_BlueHour", L"..\\Resources\\LUT\\BlueHour.cube");
	//RESOURCE_MANAGER->Load<LUT>(L"LUT_ColdChrome", L"..\\Resources\\LUT\\ColdChrome.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_CrispAutumn", L"..\\Resources\\LUT\\CrispAutumn.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_DarkAndSomber", L"..\\Resources\\LUT\\DarkAndSomber.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_Waves", L"..\\Resources\\LUT\\Waves.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_HardBoost", L"..\\Resources\\LUT\\HardBoost.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_LongBeachMorning", L"..\\Resources\\LUT\\LongBeachMorning.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_LushGreen", L"..\\Resources\\LUT\\LushGreen.cube");
	//RESOURCE_MANAGER->Load<LUT>(L"LUT_MagicHour", L"..\\Resources\\LUT\\MagicHour.cube");
	//RESOURCE_MANAGER->Load<LUT>(L"LUT_NaturalBoost", L"..\\Resources\\LUT\\NaturalBoost.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_OrangeAndBlue", L"..\\Resources\\LUT\\OrangeAndBlue.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_SoftBlackAndWhite", L"..\\Resources\\LUT\\SoftBlackAndWhite.cube");

	// Material
	shared_ptr<Material> material = make_shared<Material>();
	material->SetDiffuseMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Leather"));
	material->SetNormalMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Leather_Normal"));
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
	
	// Tank
	if (0)
	{
		shared_ptr<ClientPawn> clientPawn = make_shared<ClientPawn>("Tank");
		shared_ptr<StaticMesh> staticMesh = RESOURCE_MANAGER->Get<StaticMesh>(L"Tank");

		clientPawn->SetStaticMesh(staticMesh);
		//_clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(0.03f));
		clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(15.f, 15.f, 15.f));
		clientPawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 1.f, 0.f });
		//clientPawn->GetOrAddTransform()->SetWorldRotation({ -0.1f, 0.0f, 0.0f });

		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicShader.hlsl");
		clientPawn->GetOrAddStaticMeshRenderer()->SetShaderInfo(shaderInfo);
		SCENE->AddActor(clientPawn);
	}

	// Zelda
	if (0)
	{
		shared_ptr<ClientPawn> clientPawn = make_shared<ClientPawn>("Zelda");

		shared_ptr<StaticMesh> staticMesh = RESOURCE_MANAGER->Get<StaticMesh>(L"Zelda");
		
		clientPawn->SetStaticMesh(staticMesh);
		clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(6.f, 6.f, 6.f));
		clientPawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 3.f, 0.f });
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicShader.hlsl");
		clientPawn->GetOrAddStaticMeshRenderer()->SetShaderInfo(shaderInfo);
		SCENE->AddActor(clientPawn);
	}

	// CubeMap
	if (0)
	{
		shared_ptr<Material> cubeMapMat = make_shared<Material>();
		shared_ptr<LoadedTexture> texture = RESOURCE_MANAGER->Get<LoadedTexture>(L"Skybox");
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
		if (1) // 바닥 평면
		{
			shared_ptr<Actor> pawn = make_shared<ClientPawn>();
			pawn->Construct();

			pawn->GetOrAddTransform()->SetLocalScale({ 40.f, 40.f, 40.f });
			pawn->GetOrAddTransform()->SetLocalRotation({ 3.14f / 2, 0.0f, 0.f });
			pawn->GetOrAddTransform()->SetWorldPosition({ 0.f, -3.f, 0.f });

			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Quad");

			shared_ptr<Material> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite")->Clone();
			mat->GetDesc()->bGetIBL = 0;
			pawn->SetBasicMesh(mesh);
			pawn->SetBasicMaterial(mat);
			pawn->SetIsCastShadowedActor(false);

			SCENE->AddActor(pawn);
		}
		
		if (0) // Room
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

		// 기본 물체
		if (1)
		{
			// 큐브
			if (0)
			{
				shared_ptr<Actor> pawn = make_shared<ClientPawn>();
				pawn->Construct();

				pawn->GetOrAddTransform()->SetLocalScale({ 2.f, 2.f, 2.f });
				pawn->GetTransform()->SetLocalRotation({ 0.f, 3.14f / 4, 0.f });
				pawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 2.f, -5.f });

				shared_ptr<BasicMesh> mesh;
				mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Cube");
				shared_ptr<Material> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite");
				pawn->SetBasicMesh(mesh);
				pawn->SetBasicMaterial(mat);

				SCENE->AddActor(pawn);
			}

			// 큰 구
			if (1)
			{
				shared_ptr<Actor> pawn2 = make_shared<ClientPawn>();
				pawn2->GetOrAddTransform()->SetLocalScale({ 15.f, 15.f, 15.f });
				pawn2->GetOrAddTransform()->SetWorldPosition({ 0.f, 4.f, 0.f });

				shared_ptr<BasicMesh> mesh;
				mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Sphere");
				shared_ptr<Material> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite")->Clone();
				pawn2->SetBasicMesh(mesh);
				pawn2->SetBasicMaterial(mat);

				SCENE->AddActor(pawn2);
			}

			if (0)
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
	}

	// PostProcess
	//SCENE->AddFilter(EFilterType::Bloom);
	
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
