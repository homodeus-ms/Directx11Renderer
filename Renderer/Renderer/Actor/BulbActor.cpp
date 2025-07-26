#include "pch.h"
#include "BulbActor.h"
#include "Components/Transform.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/Material.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Managers/ShaderParameterManager.h"
#include "Managers/SceneManager.h"

BulbActor::BulbActor(const string& name)
	: Super(ELightType::Point, name)
{
}

BulbActor::~BulbActor()
{
}

void BulbActor::Construct()
{
	Super::Construct();
	
	_basicMesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Sphere");
	_material = make_shared<Material>();
	{
		MaterialDesc& desc = _material->GetMaterialDesc();
		desc.ambient = Vec4(0.0f);
		desc.diffuse = Vec4(1.f, 1.f, 0.5f, 1.f);
		desc.specular = Vec4(1.f);
		desc.emissive = Vec4(1.f, 0.8f, 0.2f, 1.f);
		desc.bUnLit = 1;
	}

	shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
	_material->SetShaderInfo(shaderInfo);
	RESOURCE_MANAGER->Add(L"Bulb", _material);

	GetTransform()->SetWorldPosition({ 0.f, 5.f, 0.f });
	//GetTransform()->SetLocalScale({ 0.5f, 0.5f, 0.5f });

	SetBasicMaterial(_material);
	SetBasicMesh(_basicMesh);
}

void BulbActor::BeginPlay()
{
	Super::BeginPlay();
}

void BulbActor::Tick()
{
	Super::Tick();

	
}
