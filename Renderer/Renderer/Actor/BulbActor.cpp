#include "pch.h"
#include "BulbActor.h"
#include "Components/Transform.h"
#include "Resource/Texture.h"
#include "Resource/BasicMesh.h"
#include "Resource/Material.h"
#include "Graphics/Shader/ShaderInfo.h"

BulbActor::BulbActor()
	: Super(ELightType::Point)
{
}

BulbActor::~BulbActor()
{
}

void BulbActor::Construct()
{
	Super::Construct();

	RESOURCE_MANAGER->Load<Texture>(L"Bulb", L"..\\Resources\\Images\\Yellow.jpg");
	_basicMesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Cube");

	_material = make_shared<Material>();
	_material->SetDiffuseMap(RESOURCE_MANAGER->Get<Texture>(L"Bulb"));
	{
		MaterialDesc& desc = _material->GetMaterialDesc();
		desc.ambient = Vec4(0.5f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
	}

	shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");
	_material->SetShaderInfo(shaderInfo);
	RESOURCE_MANAGER->Add(L"Bulb", _material);

	GetTransform()->SetWorldPosition({ 0.f, 5.f, 0.f });
	GetTransform()->SetLocalScale({ 0.5f, 0.5f, 0.5f });

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
