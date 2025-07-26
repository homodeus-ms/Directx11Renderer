#include "pch.h"
#include "Scene.h"
#include "Managers/SceneManager.h"
#include "Actor/Actor.h"
#include "Actor/Pawn.h"
#include "Actor/CameraActor.h"
#include "Actor/LightActor.h"
#include "Components/Transform.h"
#include "Components/LightComponent/LightComponent.h"
#include "Components/LightComponent/SpotLight.h"
#include "Components/LightComponent/PointLight.h"
#include "Managers/RenderManager.h"
#include "Managers/LightManager.h"
#include "Resource/Material.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Resource/BasicMesh/VertexUVBasicMesh.h"
#include "Graphics/Shader/ShaderInfo.h"


Scene::Scene()
{
	_lightManager = new LightManager();
	_commands = new CommandQueue();

	_actors.reserve(100);
	_renderedActors.reserve(100);
}

Scene::~Scene()
{
	SAFE_DELETE(_lightManager);
	SAFE_DELETE(_commands);
}

void Scene::Construct()
{
	// LightManager
	RegisterActor(_lightManager->AddDefaultDirectionalLight());
	_onLightManagerCreated.Broadcast();

	// ShadowMap
	GET_SINGLE(RenderManager)->SetShadowMap(GRAPHICS->GetShadowMap());
	CreateShadowMapDebugActor();

	unordered_set<shared_ptr<Actor>> actors = _actors;
	for (const shared_ptr<Actor>& actor : actors)
	{
		actor->Construct();
	}

	_mainCamera = make_shared<CameraActor>();
	_mainCamera->Construct();
	_currCamera = _mainCamera;
	_topViewCamera = make_shared<CameraActor>();
	_topViewCamera->Construct();
	{
		_topViewCamera->GetTransform()->SetWorldPosition(Vec3(0.f, 20.f, -30.f));
		Vec3 targetLook = { 0.f, -1.f, 2.f };
		_topViewCamera->GetTransform()->SetLocalRotationByTargetLook(targetLook);
		Vec3 look = _topViewCamera->GetTransform()->GetLook();
	}

	//_mainCamera->_onCameraLookChanged.BindObject(shared_from_this(), &Scene::OnMainCameraLookChangedCallback);
	//_lightManager->SetGlobalLightLook(_mainCamera->GetTransform()->GetLook());
}

void Scene::BeginPlay()
{
	// Actors
	for (const shared_ptr<Actor>& actor : _actors)
	{
		actor->BeginPlay();
	}

	// Camera
	_mainCamera->BeginPlay();
	_topViewCamera->BeginPlay();
}

void Scene::Tick()
{
	_commands->TickCommands();
	
	for (const shared_ptr<Actor>& actor : _actors)
	{
		actor->Tick();
	}

	_currCamera->Tick();

}

void Scene::LateTick()
{
	for (const shared_ptr<Actor>& actor : _actors)
	{
		actor->LateTick();
	}
}

void Scene::Render()
{
	GET_SINGLE(RenderManager)->Render(_renderedActors);
}

void Scene::AddActor(shared_ptr<Actor> actor)
{
	_commands->PushCommand([this, actor]()
		{
			auto self = shared_from_this();

			if (actor->GetActorType() == EActorType::LightActor)
			{
				CheckAndAddLightActor(actor);
			}
			else
			{
				RegisterActor(actor);
				actor->Construct();
				actor->BeginPlay();
			}
		});
}

void Scene::RemoveActor(shared_ptr<Actor> actor)
{
	if (IsLightActor(actor))
	{
		_lightManager->ReduceLight(static_pointer_cast<LightActor>(actor));
	}

	_commands->PushCommand([this, actor]()
		{
			auto self = shared_from_this();
			DeregisterActor(actor);
		});
}

vector<shared_ptr<Actor>> Scene::GetRenderedActors()
{
	return _renderedActors;
}

shared_ptr<LightActor> Scene::GetGlobalLight()
{
	return _lightManager->GetGlobalLight();
}

Vec3 Scene::GetCurrCameraLook() const
{
	return _currCamera->GetTransform()->GetLook();
}

Matrix Scene::GetCurrCameraV()
{
	return _currCamera->GetV();
}

Matrix Scene::GetCurrCameraP()
{
	return _currCamera->GetP();
}

Matrix Scene::GetCurrCameraVP()
{
	return _currCamera->GetCameraVP();
}

shared_ptr<CameraActor> Scene::SwitchCameraAndGet(ECameraType type)
{
	if (type == ECameraType::MainCamera)
		_currCamera = _mainCamera;
	else
		_currCamera = _topViewCamera;

	return _currCamera;
}

void Scene::TurnGlobalLightOnOff(bool bTurnOn)
{
	_lightManager->TurnDirectionalLightOnOff(bTurnOn);
}

void Scene::CreateEnvironment(const wstring& textureName, bool bSetEnvLighting)
{
	if (_cubeMapCached != nullptr)
	{
		RemoveActor(_cubeMapCached);
	}

	_cubeMapCached = make_shared<Actor>(EActorType::Actor, "CubeMap");
	shared_ptr<LoadedTexture> tex = RESOURCE_MANAGER->Get<LoadedTexture>(textureName);
	if (tex == nullptr)
	{
		LOG(Log, "Can't find Env Texture");
		return;
	}

	shared_ptr<Material> cubeMapMat = make_shared<Material>();
	cubeMapMat->SetDiffuseMap(tex);
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
	
	_cubeMapCached->Construct();

	shared_ptr<BasicMesh> mesh;
	mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"CubeMap");
	_cubeMapCached->SetBasicMesh(mesh);
	_cubeMapCached->SetBasicMaterial(cubeMapMat);

	AddActor(_cubeMapCached);

	if (bSetEnvLighting)
		SetEnvLightTexture(textureName);
}

void Scene::SetEnvLightTexture(const wstring& textureName)
{
	_lightManager->SetEnvLightTexture(textureName);
}

void Scene::TurnEnvLightOnOff(bool bOn)
{
	_lightManager->TurnEnvLightOnOff(bOn);
}

shared_ptr<LightActor> Scene::AddSpotLight()
{
	const shared_ptr<LightActor>& added = _lightManager->IncreaseSpotLightOrNull();
	if (added != nullptr)
		AddLightActor(added);

	return added;
}

shared_ptr<LightActor> Scene::AddPointLightOrNull()
{
	const shared_ptr<LightActor>& added = _lightManager->IncreasePointLightOrNull();
	if (added != nullptr)
		AddLightActor(added);

	return added;
}

void Scene::RemoveLight(shared_ptr<LightActor> actor)
{
	RemoveActor(actor);
}

void Scene::OnMainCameraLookChangedCallback(const Vec3& look)
{
	//_lightManager->SetGlobalLightLook(look);
}

void Scene::RegisterActor(const shared_ptr<Actor>& actor)
{
	_actors.insert(actor);
	if (actor->IsRenderedActor())
	{
		_renderedActors.push_back(actor);
		_onRenderedActorRegistered.Broadcast(actor);
	}
}

void Scene::DeregisterActor(const shared_ptr<Actor>& actor)
{
	if (actor->IsRenderedActor())
	{
		auto findIt = std::find(_renderedActors.begin(), _renderedActors.end(), actor);
		if (findIt != _renderedActors.end())
			_renderedActors.erase(findIt);
	}
	_actors.erase(actor);
}

bool Scene::IsLightActor(const shared_ptr<Actor>& actor)
{
	return actor->GetActorType() == EActorType::LightActor;
}

void Scene::CheckAndAddLightActor(const shared_ptr<Actor>& actor)
{
	ELightType lightType = static_pointer_cast<LightActor>(actor)->GetLightType();
	switch (lightType)
	{
	case ELightType::Spot:
		_lightManager->IncreseSpotLightCount();
		AddLightActor(actor);
		return;
	case ELightType::Point:
		_lightManager->IncresePointLightCount();
		AddLightActor(actor);
		return;
	default:
		return;
	}
}

void Scene::AddLightActor(shared_ptr<Actor> actor)
{
	_commands->PushCommand([this, actor]()
		{
			auto self = shared_from_this();
			RegisterActor(actor);
			actor->Construct();
			actor->BeginPlay();
		});
}

void Scene::CreateShadowMapDebugActor()
{
	_shadowMapDebugActor = make_shared<Actor>(EActorType::DebugActor, "ShadowDebug");
	
	_shadowMapDebugActor->Construct();
	_shadowMapDebugActor->GetTransform()->SetLocalScale({ 15.f, 15.f, 1.f });
	_shadowMapDebugActor->GetTransform()->SetWorldPosition({ 0.f, 0.f, -10.f });
	

	shared_ptr<VertexUVBasicMesh> mesh = make_shared<VertexUVBasicMesh>();
	mesh->CreateQuad();

	shared_ptr<Material> material = make_shared<Material>();

	shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"DebugShader.hlsl");
	material->SetShaderInfo(shaderInfo);
	RESOURCE_MANAGER->Add(L"ShadowMapDebugMaterial", material);

	_shadowMapDebugActor->SetBasicMesh(mesh);
	_shadowMapDebugActor->SetBasicMaterial(material);
	_shadowMapDebugActor->BeginPlay();

	_renderedActors.push_back(_shadowMapDebugActor);
}

