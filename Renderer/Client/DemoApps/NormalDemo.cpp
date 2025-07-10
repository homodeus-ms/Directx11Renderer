#include "pch.h"
#include "NormalDemo.h"
#include "Main/ClientPawn.h"
#include "Resource/Mesh.h"
#include "Resource/Texture.h"
#include "Components/Transform.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Actor/CameraActor.h"
#include "Resource/Material.h"

void NormalDemo::Construct()
{
	// 이쪽에서 정해줄 수 있게 하는 것들은 나중에 UI로 빼기 쉬움

	// Resource Load
	RESOURCE_MANAGER->Load<Texture>(L"Guitarist", L"..\\Resources\\Images\\Guitarist2.png");

	// Material
	shared_ptr<Material> material = make_shared<Material>();
	material->SetDiffuseMap(RESOURCE_MANAGER->Get<Texture>(L"Guitarist"));
	{
		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(0.5f);
		desc.diffuse = Vec4(1.f);
		desc.specular = Vec4(1.f);
		
	}
	shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"03. LightTest.hlsl");
	material->SetShaderInfo(shaderInfo);
	RESOURCE_MANAGER->Add(L"Guitarist", material);

	// Client Pawn
	{
		shared_ptr<Actor> pawn = make_shared<ClientPawn>();
		pawn->AddRenderComponent();
		pawn->Construct();
		pawn->_actorName = "ClientPawn";
		pawn->GetTransform()->SetWorldPosition({ -1.f, 0.f, 2.f });

		shared_ptr<Mesh> mesh;
		mesh = RESOURCE_MANAGER->Get<Mesh>(L"Cube");

		pawn->SetMesh(mesh);
		pawn->SetMaterial(material);
		_actors.push_back(pawn);
	}

	{
		shared_ptr<Actor> pawn = make_shared<ClientPawn>();
		pawn->AddRenderComponent();
		pawn->Construct();
		pawn->_actorName = "ClientPawn2";
		pawn->GetTransform()->SetWorldPosition({ 1.f, 0.f, 2.f });

		shared_ptr<Mesh> mesh;
		mesh = RESOURCE_MANAGER->Get<Mesh>(L"Cube");

		pawn->SetMesh(mesh);
		pawn->SetMaterial(material);
		_actors.push_back(pawn);
	}


	// Scene
	SCENE_MANAGER->AddActors(_actors);

}

void NormalDemo::BeginPlay()
{
	SHADER_PARAM_MANAGER->BeginPlay();
	//_actors.at(0)->BeginPlay();
}

void NormalDemo::Tick()
{
	//SHADER_PARAM_MANAGER->Update();

	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.5f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(1.f);
		
		lightDesc.direction = Vec3(0.0f, 0.f, 1.f);
		SHADER_PARAM_MANAGER->PushLightData(lightDesc);
	}

}

void NormalDemo::Render()
{
	
}
