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
#include "Resource/Texture.h"
#include "Graphics/Shader/ShaderInfo.h"


Scene::Scene()
{
	_lightManager = new LightManager();
	_commands = new CommandQueue();
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

	unordered_set<shared_ptr<Actor>> actors = _actors;
	for (const shared_ptr<Actor>& actor : actors)
	{
		actor->Construct();
	}

	_mainCamera = make_shared<CameraActor>();
	_mainCamera->Construct();

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

}

void Scene::Tick()
{
	_commands->TickCommands();
	
	for (const shared_ptr<Actor>& actor : _actors)
	{
		actor->Tick();
	}

	_mainCamera->Tick();

	// 여기서 actor들 Render
	for (shared_ptr<Actor> actor : _actors)
	{
		actor->Render();
	}
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
	vector<shared_ptr<Actor>> temp;
	temp.insert(temp.end(), _actors.begin(), _actors.end());
	GET_SINGLE(RenderManager)->Render(temp);
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
	vector<shared_ptr<Actor>> actors;
	for (const shared_ptr<Actor>& actor : _actors)
	{
		if (actor->IsRenderedActor())
			actors.push_back(actor);
	}

	return actors;
}

shared_ptr<LightActor> Scene::GetGlobalLight()
{
	return _lightManager->GetGlobalLight();
}

Vec3 Scene::GetMainCameraLook() const
{
	return _mainCamera->GetTransform()->GetLook();
}

Matrix Scene::GetMainCameraVP()
{
	return _mainCamera->GetCameraVP();
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
	shared_ptr<Texture> tex = RESOURCE_MANAGER->Get<Texture>(textureName);
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
		_onRenderedActorRegistered.Broadcast(actor);
}

void Scene::DeregisterActor(const shared_ptr<Actor>& actor)
{
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

