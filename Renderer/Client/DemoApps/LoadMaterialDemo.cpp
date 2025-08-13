#include "pch.h"
#include "LoadMaterialDemo.h"
#include "Utils/FileUtils.h"
#include "Main/ClientPawn.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/BasicMesh/DefaultBasicMesh.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Resource/LUT.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Actor/CameraActor.h"
#include "Resource/Material/MaterialBase.h"
#include "Resource/Material/Material.h"
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
	g_FlowManager->BeginPlay();


	LoadMaterials();
	LoadDDSs();
	LoadLUTs();
	CreateBasicObjects();
	CreateStaticModels();

}

void LoadMaterialDemo::BeginPlay()
{
}

void LoadMaterialDemo::Tick()
{
}

void LoadMaterialDemo::Render()
{
}

void LoadMaterialDemo::LoadDDSs()
{
	// LDR DDS
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"NightCity1_spec", L"..\\Resources\\Images\\DDS\\NightCity1\\NightCity1_spec.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"NightCity1_diff", L"..\\Resources\\Images\\DDS\\NightCity1\\NightCity1_diff.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"NightCity2_spec", L"..\\Resources\\Images\\DDS\\NightCity2\\NightCity2_spec.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"NightCity2_diff", L"..\\Resources\\Images\\DDS\\NightCity2\\NightCity2_diff.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"NightPath_spec", L"..\\Resources\\Images\\DDS\\NightPath\\NightPath_spec.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"NightPath_diff", L"..\\Resources\\Images\\DDS\\NightPath\\NightPath_diff.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"Museum_spec", L"..\\Resources\\Images\\DDS\\Museum\\Museum_spec.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"Museum_diff", L"..\\Resources\\Images\\DDS\\Museum\\Museum_diff.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"SnowForest_spec", L"..\\Resources\\Images\\DDS\\SnowForest\\SnowForest_spec.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"SnowForest_diff", L"..\\Resources\\Images\\DDS\\SnowForest\\SnowForest_diff.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"DawnField_spec", L"..\\Resources\\Images\\DDS\\DawnField\\DawnField_spec.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"DawnField_diff", L"..\\Resources\\Images\\DDS\\DawnField\\DawnField_diff.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"DayLight1_spec", L"..\\Resources\\Images\\DDS\\DayLight1\\DayLight1_spec.dds");
	//RESOURCE_MANAGER->Load<LoadedTexture>(L"DayLight1_diff", L"..\\Resources\\Images\\DDS\\DayLight1\\DayLight1_diff.dds");

	// DDS (HDRI)
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Cloudy", L"..\\Resources\\Images\\DDS_HDRI\\Cloudy\\CloudyEnvHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Cloudy_spec", L"..\\Resources\\Images\\DDS_HDRI\\Cloudy\\CloudySpecularHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Cloudy_diff", L"..\\Resources\\Images\\DDS_HDRI\\Cloudy\\CloudyDiffuseHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Cloudy_brdf", L"..\\Resources\\Images\\DDS_HDRI\\Cloudy\\CloudyBrdf.dds");

	RESOURCE_MANAGER->Load<LoadedTexture>(L"InsideGym", L"..\\Resources\\Images\\DDS_HDRI\\InsideGym\\InsideGymEnvHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"InsideGym_spec", L"..\\Resources\\Images\\DDS_HDRI\\InsideGym\\InsideGymSpecularHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"InsideGym_diff", L"..\\Resources\\Images\\DDS_HDRI\\InsideGym\\InsideGymDiffuseHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"InsideGym_brdf", L"..\\Resources\\Images\\DDS_HDRI\\InsideGym\\InsideGymBrdf.dds");


	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightEnv1", L"..\\Resources\\Images\\DDS_HDRI\\NightEnv1\\NightEnv1EnvHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightEnv1_spec", L"..\\Resources\\Images\\DDS_HDRI\\NightEnv1\\NightEnv1SpecularHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightEnv1_diff", L"..\\Resources\\Images\\DDS_HDRI\\NightEnv1\\NightEnv1DiffuseHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"NightEnv1_brdf", L"..\\Resources\\Images\\DDS_HDRI\\NightEnv1\\NightEnv1Brdf.dds");

	RESOURCE_MANAGER->Load<LoadedTexture>(L"EveningSky", L"..\\Resources\\Images\\DDS_HDRI\\EveningSky\\EveningSkyEnvHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"EveningSky_spec", L"..\\Resources\\Images\\DDS_HDRI\\EveningSky\\EveningSkySpecularHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"EveningSky_diff", L"..\\Resources\\Images\\DDS_HDRI\\EveningSky\\EveningSkyDiffuseHDR.dds");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"EveningSky_brdf", L"..\\Resources\\Images\\DDS_HDRI\\EveningSky\\EveningSkyBrdf.dds");
}

void LoadMaterialDemo::LoadLUTs()
{
	// LUT
	RESOURCE_MANAGER->Load<LUT>(L"LUT_BlueArchitecture", L"..\\Resources\\LUT\\BlueArchitecture.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_CrispAutumn", L"..\\Resources\\LUT\\CrispAutumn.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_DarkAndSomber", L"..\\Resources\\LUT\\DarkAndSomber.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_HardBoost", L"..\\Resources\\LUT\\HardBoost.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_LongBeachMorning", L"..\\Resources\\LUT\\LongBeachMorning.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_LushGreen", L"..\\Resources\\LUT\\LushGreen.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_OrangeAndBlue", L"..\\Resources\\LUT\\OrangeAndBlue.cube");
	RESOURCE_MANAGER->Load<LUT>(L"LUT_SoftBlackAndWhite", L"..\\Resources\\LUT\\SoftBlackAndWhite.cube");
}

void LoadMaterialDemo::LoadMaterials()
{
	LoadTextures();

	// Material
	{
		shared_ptr<MaterialBase> material = make_shared<Material>();
		material->SetAlbedoMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Red_Chipped_Albedo"));
		material->SetAOMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Red_Chipped_AO"));
		material->SetHeightMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Red_Chipped_Height"));
		material->SetMetallicRoughnessMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Red_Chipped_MetallicRoughness"));
		material->SetNormalMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Red_Chipped_Normal"));
		//material->SetRoughnessMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Red_Chipped_Roughness"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.5f);
			desc.diffuse = Vec4(1.f);
			desc.specular = Vec4(1.f, 1.f, 1.f, 10.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
			desc.MaterialType = EMaterialType::Default;
			desc.UsePBR();
		}

		
		RESOURCE_MANAGER->Add(L"Metal_Red_Chipped", material);
	}

	{
		shared_ptr<MaterialBase> material = make_shared<Material>();
		material->SetAlbedoMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Green_Chipped_Albedo"));
		material->SetAOMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Green_Chipped_AO"));
		material->SetHeightMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Green_Chipped_Height"));
		material->SetMetallicRoughnessMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Green_Chipped_MetallicRoughness"));
		material->SetNormalMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Green_Chipped_Normal"));
		//material->SetRoughnessMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Green_Chipped_Roughness"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.5f);
			desc.diffuse = Vec4(1.f);
			desc.specular = Vec4(1.f, 1.f, 1.f, 10.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
			desc.MaterialType = EMaterialType::Default;
			desc.UsePBR();
		}

		
		RESOURCE_MANAGER->Add(L"Metal_Green_Chipped", material);
	}

	{
		shared_ptr<MaterialBase> material = make_shared<Material>();
		material->SetAlbedoMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Dented_Albedo"));
		material->SetAOMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Dented_AO"));
		material->SetHeightMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Dented_Height"));
		material->SetMetallicRoughnessMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Dented_MetallicRoughness"));
		material->SetNormalMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Dented_Normal"));
		//material->SetRoughnessMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Metal_Dented_Roughness"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.5f);
			desc.diffuse = Vec4(1.f);
			desc.specular = Vec4(1.f, 1.f, 1.f, 10.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
			desc.MaterialType = EMaterialType::Default;
			desc.UsePBR();
		}
		
		RESOURCE_MANAGER->Add(L"Metal_Dented", material);
	}

	// Rock Grey Porous
	{
		shared_ptr<MaterialBase> material = make_shared<Material>();
		material->SetAlbedoMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Rock_Grey_Porous_Albedo"));
		material->SetAOMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Rock_Grey_Porous_AO"));
		material->SetHeightMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Rock_Grey_Porous_Height"));
		material->SetMetallicRoughnessMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Rock_Grey_Porous_MetallicRoughness"));
		material->SetNormalMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Rock_Grey_Porous_Normal"));
		//material->SetRoughnessMap(RESOURCE_MANAGER->Get<LoadedTexture>(L"Rock_Grey_Porous_Roughness"));
		{
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.5f);
			desc.diffuse = Vec4(1.f);
			desc.specular = Vec4(1.f, 1.f, 1.f, 10.f);
			desc.emissive = Vec4(0.f, 0.f, 0.0f, 1.f);
			desc.bUnLit = 0;
			desc.MaterialType = EMaterialType::Default;
			desc.UsePBR();
		}
		
		RESOURCE_MANAGER->Add(L"Rock_Grey_Porous", material);
		
	}
}

void LoadMaterialDemo::LoadTextures()
{
	// Metal_Red_Chipped
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Red_Chipped_Albedo", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Red_Chipped\\Metal_Red_Chipped_Albedo.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Red_Chipped_AO", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Red_Chipped\\Metal_Red_Chipped_AO.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Red_Chipped_Height", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Red_Chipped\\Metal_Red_Chipped_Height.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Red_Chipped_MetallicRoughness", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Red_Chipped\\Metal_Red_Chipped_MetallicRoughness.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Red_Chipped_Normal", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Red_Chipped\\Metal_Red_Chipped_Normal.png");
	
	// Metal_Green_Chipped
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Green_Chipped_Albedo", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Green_Chipped\\Metal_Green_Chipped_Albedo.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Green_Chipped_AO", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Green_Chipped\\Metal_Green_Chipped_AO.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Green_Chipped_Height", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Green_Chipped\\Metal_Green_Chipped_Height.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Green_Chipped_MetallicRoughness", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Green_Chipped\\Metal_Green_Chipped_MetallicRoughness.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Green_Chipped_Normal", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Green_Chipped\\Metal_Green_Chipped_Normal.png");
	

	// Metal_Dented
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Dented_Albedo", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Dented\\Metal_Dented_Albedo.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Dented_AO", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Dented\\Metal_Dented_AO.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Dented_Height", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Dented\\Metal_Dented_Height.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Dented_MetallicRoughness", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Dented\\Metal_Dented_MetallicRoughness.png");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Metal_Dented_Normal", L"..\\Resources\\Images\\PBR\\Metal\\Metal_Dented\\Metal_Dented_Normal.png");
	

	// Grey Stone
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Rock_Grey_Porous_Albedo", L"..\\Resources\\Images\\PBR\\Rock\\Rock_Grey_Porous\\Rock_Grey_Porous_Albedo.jpg");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Rock_Grey_Porous_AO", L"..\\Resources\\Images\\PBR\\Rock\\Rock_Grey_Porous\\Rock_Grey_Porous_AO.jpg");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Rock_Grey_Porous_Height", L"..\\Resources\\Images\\PBR\\Rock\\Rock_Grey_Porous\\Rock_Grey_Porous_Height.jpg");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Rock_Grey_Porous_MetallicRoughness", L"..\\Resources\\Images\\PBR\\Rock\\Rock_Grey_Porous\\Rock_Grey_Porous_MetallicRoughness.jpg");
	RESOURCE_MANAGER->Load<LoadedTexture>(L"Rock_Grey_Porous_Normal", L"..\\Resources\\Images\\PBR\\Rock\\Rock_Grey_Porous\\Rock_Grey_Porous_Normal.jpg");
	
}

void LoadMaterialDemo::CreateBasicObjects()
{
	// 바닥 평면
	if (1)
	{
		shared_ptr<Actor> pawn = make_shared<ClientPawn>("Floor");
		pawn->Construct();

		//pawn->GetOrAddTransform()->SetLocalScale({ 40.f, 40.f, 40.f });
		//pawn->GetOrAddTransform()->SetLocalRotation({ 3.14f / 2, 0.0f, 0.f });
		pawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 0.f, 0.f });

		shared_ptr<BasicMesh> mesh;
		mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Grid");

		shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite")->Clone();
		mat->GetDesc()->bGetIBL = 0;
		pawn->SetBasicMesh(mesh);
		pawn->SetBasicMaterial(mat);
		pawn->SetIsCastShadowedActor(false);
		

		SCENE->AddActor(pawn);
	}

	// Room
	if (0)
	{
		shared_ptr<Actor> pawn = make_shared<ClientPawn>();
		pawn->Construct();

		pawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 0.f, 0.f });

		shared_ptr<BasicMesh> mesh = make_shared<DefaultBasicMesh>();
		mesh->CreateSquareRoom();

		shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite");
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
			shared_ptr<Actor> pawn = make_shared<ClientPawn>("SmallCube");
			pawn->Construct();

			pawn->GetOrAddTransform()->SetLocalScale({ 2.f, 2.f, 2.f });
			pawn->GetTransform()->SetLocalRotation({ 0.f, 3.14f / 2, 0.f });
			pawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 4.f, 0.f });

			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Cube");
			shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite");
			pawn->SetBasicMesh(mesh);
			pawn->SetBasicMaterial(mat);

			SCENE->AddActor(pawn);
		}

		// 거울
		if (1)
		{
			shared_ptr<Actor> mirror = make_shared<ClientPawn>(EActorType::ReflectActor, "Mirror");
			mirror->Construct();
			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Quad");
			shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite")->Clone();

			mirror->SetBasicMesh(mesh);
			mirror->SetBasicMaterial(mat);

			mirror->GetOrAddTransform()->SetLocalScale({ 18.f, 18.f, 18.f });
			mirror->GetTransform()->SetLocalRotation({ 0.f, 3.14f / 4, 0.f });
			mirror->GetOrAddTransform()->SetWorldPosition({ 5.f, 10.f, 7.f });
			mirror->SetIsCastShadowedActor(false);

			SCENE->AddActor(mirror);
		}

		if (1)
		{
			shared_ptr<Actor> mirror = make_shared<ClientPawn>(EActorType::ReflectActor, "Mirror2");
			mirror->Construct();
			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Quad");
			shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite")->Clone();

			mirror->SetBasicMesh(mesh);
			mirror->SetBasicMaterial(mat);
			
			mirror->GetOrAddTransform()->SetLocalScale({ 18.f, 18.f, 18.f });
			//mirror->GetTransform()->SetLocalRotation({ 0.f, -3.14f / 2, 0.f });
			mirror->GetOrAddTransform()->SetWorldPosition({ -10.f, 10.f, 5.f });
			mirror->SetIsCastShadowedActor(false);

			SCENE->AddActor(mirror);
		}

		// 큰 구
		if (0)
		{
			shared_ptr<Actor> pawn2 = make_shared<ClientPawn>("LargeSphere");
			pawn2->GetOrAddTransform()->SetLocalScale({ 8.f, 8.f, 8.f });
			//pawn2->GetTransform()->SetLocalRotation({ 0.f, -3.14f / 1.5, 0.f });
			pawn2->GetOrAddTransform()->SetWorldPosition({ 0.f, 4.f, 0.f });

			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Sphere");
			//mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"CustomSphere");
			//shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"BasicWhite")->Clone();
			//shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"Metal_Red_Chipped")->Clone();
			//shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"Metal_Dented")->Clone();
			//shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"Metal_Green_Chipped")->Clone();
			shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"Rock_Grey_Porous")->Clone();
			pawn2->SetBasicMesh(mesh);
			pawn2->SetBasicMaterial(mat);
			//pawn2->SetDrawNormal(true);

			SCENE->AddActor(pawn2);
		}

		// 작은 구
		if (1)
		{
			shared_ptr<Actor> pawn2 = make_shared<ClientPawn>("SmallSphere");
			pawn2->Construct();
			//pawn2->GetOrAddTransform()->SetLocalScale({ 3.f, 3.f, 3.f });
			pawn2->GetOrAddTransform()->SetWorldPosition({ 3.f, 5.f, 0.f });

			shared_ptr<BasicMesh> mesh;
			mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Sphere");
			//shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"BasicRed")->Clone();
			shared_ptr<MaterialBase> mat = RESOURCE_MANAGER->Get<Material>(L"Metal_Dented")->Clone();
			pawn2->SetBasicMesh(mesh);
			pawn2->SetBasicMaterial(mat);

			SCENE->AddActor(pawn2);
		}
	}
}

void LoadMaterialDemo::CreateStaticModels()
{
	//RESOURCE_MANAGER->LoadMeshFromAssetFolder(L"Tank", L"Tank");
	//RESOURCE_MANAGER->LoadMeshFromAssetFolder(L"Zelda", L"Zelda");
	//
	if (1)
	{
		RESOURCE_MANAGER->LoadMeshFromAssetFolder(L"Knight", L"Knight", true, true);
		RESOURCE_MANAGER->LoadMeshFromAssetFolder(L"DamagedHelmet", L"DamagedHelmet", true, true);

		// Knight
		if (1)
		{
			shared_ptr<ClientPawn> clientPawn = make_shared<ClientPawn>("Knight");
			shared_ptr<StaticMesh> staticMesh = RESOURCE_MANAGER->Get<StaticMesh>(L"Knight");
			vector<shared_ptr<MaterialBase>> materials = staticMesh->GetMaterials();

			clientPawn->SetStaticMesh(staticMesh);
			//_clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(0.03f));
			clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(8.f, 8.f, 8.f));
			clientPawn->GetOrAddTransform()->SetWorldPosition({ -3.f, 4.f, 0.f });
			clientPawn->GetOrAddTransform()->SetWorldRotation({ 3.14f / 2, -3.14f, 0.0f });
			
			//clientPawn->SetDrawNormal(true);
			SCENE->AddActor(clientPawn);
		}

		// Damaged Helmet
		if (0)
		{
			shared_ptr<ClientPawn> clientPawn = make_shared<ClientPawn>("DamagedHelmet");
			shared_ptr<StaticMesh> staticMesh = RESOURCE_MANAGER->Get<StaticMesh>(L"DamagedHelmet");
			vector<shared_ptr<MaterialBase>> materials = staticMesh->GetMaterials();

			clientPawn->SetStaticMesh(staticMesh);
			clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(6.f, 6.f, 6.f));
			clientPawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 2.f, 0.f });
			clientPawn->GetOrAddTransform()->SetWorldRotation({ 3.14f / 2, 0.0f, 0.0f });
			
			//clientPawn->SetDrawNormal(true);
			SCENE->AddActor(clientPawn);
		}
	}

	


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

		SCENE->AddActor(clientPawn);
	}

	// Zelda
	if (0)
	{
		shared_ptr<ClientPawn> clientPawn = make_shared<ClientPawn>("Zelda");

		shared_ptr<StaticMesh> staticMesh = RESOURCE_MANAGER->Get<StaticMesh>(L"Zelda");

		clientPawn->SetStaticMesh(staticMesh);
		clientPawn->GetOrAddTransform()->SetLocalScale(Vec3(8.f, 8.f, 8.f));
		clientPawn->GetOrAddTransform()->SetWorldPosition({ 0.f, 3.f, 0.f });
		
		SCENE->AddActor(clientPawn);
	}
}
