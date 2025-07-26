#include "pch.h"
#include "LightActor.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/DirectionalLight.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"
#include "Components/Transform.h"
#include "Graphics/RenderPass/ShadowMapResources.h"
#include "Resource/Material.h"
#include "Resource/Texture/LoadedTexture.h"


LightActor::LightActor(ELightType lightType, const string& name)
	: Super(EActorType::LightActor, name), _lightType(lightType)
{
	switch (_lightType)
	{
	case ELightType::Directional:
		_lightComponent = make_shared<DirectionalLight>();
		break;
	case ELightType::Spot:
		_lightComponent = make_shared<SpotLight>();
		break;
	case ELightType::Point:
		_lightComponent = make_shared<PointLight>();
		break;
	default:
		assert(false);
		break;
	}
	
}

LightActor::~LightActor()
{	
	int a = 3;
}

LightDesc* LightActor::GetDesc() const
{
	return _lightComponent->GetDesc();
}

float LightActor::GetRange()
{
	return _lightComponent->GetRange();
}

void LightActor::Construct()
{
	Super::Construct();

	assert(_lightComponent != nullptr);
	AddComponent(_lightComponent);

	GetOrAddTransform()->SetLocalScale({ 0.5f, 0.5f, 0.5f });

	GetOrAddBasicMeshRenderer();
	shared_ptr<BasicMesh> mesh;
	mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Sphere");
	
	shared_ptr<Material> mat = RESOURCE_MANAGER->Get<Material>(L"BasicYellow")->Clone();
	mat->GetDesc()->bUnLit = 1;
	if (GetLightType() == ELightType::Spot)
		mat->GetDesc()->diffuse = { 1.f, 0.7f, 0.f, 1.f };
	else if (GetLightType() == ELightType::Point)
		mat->GetDesc()->diffuse = { 1.f, 0.5f, 0.5f, 1.f };

	SetBasicMesh(mesh);
	SetBasicMaterial(mat);
}

void LightActor::BeginPlay()
{
	Super::BeginPlay();

}

void LightActor::Tick()
{
	Super::Tick();
}

void LightActor::SetOnOff(bool bIsOn)
{
	_bIsOn = bIsOn;
	
	_lightComponent->SetIsOn(bIsOn);

}

array<Matrix, 6> LightActor::GetLightVPForPointLight()
{
	array<Matrix, 6> VPs;

	Vec3 pos = GetTransform()->GetWorldPosition();
	// +x, -x, +y, -y, +z, -z
	Matrix v1 = XMMatrixLookAtLH(pos, pos + Vec3(1, 0, 0), Vec3(0, 1, 0));
	Matrix v2 = XMMatrixLookAtLH(pos, pos + Vec3(-1, 0, 0), Vec3(0, 1, 0));
	Matrix v3 = XMMatrixLookAtLH(pos, pos + Vec3(0, 1, 0), Vec3(0, 0, -1));
	Matrix v4 = XMMatrixLookAtLH(pos, pos + Vec3(0, -1, 0), Vec3(0, 0, 1));
	Matrix v5 = XMMatrixLookAtLH(pos, pos + Vec3(0, 0, 1), Vec3(0, 1, 0));
	Matrix v6 = XMMatrixLookAtLH(pos, pos + Vec3(0, 0, -1), Vec3(0, 1, 0));
	
	Matrix P = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.f, 1.f, 100.f);
	
	VPs[0] = v1 * P;
	VPs[1] = v2 * P;
	VPs[2] = v3 * P;
	VPs[3] = v4 * P;
	VPs[4] = v5 * P;
	VPs[5] = v6 * P;

	return VPs;
}

bool LightActor::IsThisPointLightUseShadowMap()
{
	if (_lightType != ELightType::Point)
		return false;

	PointLightDesc* desc = static_cast<PointLightDesc*>(GetDesc());
	if (desc->bShadowMapUsing == 1)
		return true;

	return false;
}

void LightActor::GetLightVPForPointLight(OUT vector<Matrix>& views, OUT vector<Matrix>& VPs)
{
	vector<Matrix> vpVector(6);

	Vec3 pos = GetTransform()->GetWorldPosition();
	// +x, -x, +y, -y, +z, -z
	views.push_back(XMMatrixLookAtLH(pos, pos + Vec3(1, 0, 0), Vec3(0, 1, 0)));
	views.push_back(XMMatrixLookAtLH(pos, pos + Vec3(-1, 0, 0), Vec3(0, 1, 0)));
	views.push_back(XMMatrixLookAtLH(pos, pos + Vec3(0, 1, 0), Vec3(0, 0, -1)));
	views.push_back(XMMatrixLookAtLH(pos, pos + Vec3(0, -1, 0), Vec3(0, 0, 1)));
	views.push_back(XMMatrixLookAtLH(pos, pos + Vec3(0, 0, 1), Vec3(0, 1, 0)));
	views.push_back(XMMatrixLookAtLH(pos, pos + Vec3(0, 0, -1), Vec3(0, 1, 0)));

	Matrix P = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.f, 1.f, 100.f);

	VPs.push_back(views[0] * P);
	VPs.push_back(views[1] * P);
	VPs.push_back(views[2] * P);
	VPs.push_back(views[3] * P);
	VPs.push_back(views[4] * P);
	VPs.push_back(views[5] * P);
}

void LightActor::UpdateVP()
{
	Vec3 pos = GetTransform()->GetWorldPosition();
	Vec3 look = GetTransform()->GetLook();
	look.Normalize();
	Vec3 at = pos + look;
	Vec3 up = GetTransform()->GetUp();
	up.Normalize();

	_matV = XMMatrixLookAtLH(pos, at, up);

	float x = static_cast<float>(VIEW_X);
	float y = static_cast<float>(VIEW_Y);

	if (_lightType == ELightType::Directional)
	{
		_matP = XMMatrixOrthographicLH(x, y, 0.1f, 100.f);
	}
	else if (_lightType == ELightType::Spot)
	{
		_matP = XMMatrixPerspectiveFovLH(3.14f / 2, 1.f, 1.f, 100.f);
	}
	
}

void LightActor::SetShadowMapIndex(int32 index)
{
	LightDesc* lightDesc = GetDesc();

	if (_lightType == ELightType::Directional)
	{
		DirectionalLightDesc* desc = static_cast<DirectionalLightDesc*>(lightDesc);
		desc->shadowMapIndex = index;
	}
	else if (_lightType == ELightType::Spot)
	{
		SpotLightDesc* desc = static_cast<SpotLightDesc*>(lightDesc);
		desc->shadowMapIndex = index;
	}
}
