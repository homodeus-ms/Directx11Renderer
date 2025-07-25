#include "pch.h"
#include "CameraDemo.h"
#include "Main/ClientPawn.h"
#include "Resource/BasicMesh/BasicMesh.h"
#include "Resource/Texture.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/ConstantBuffer.h"
#include "Actor/CameraActor.h"
#include "Resource/Material.h"

void CameraDemo::Construct()
{
	// 이쪽에서 정해줄 수 있게 하는 것들은 나중에 UI로 빼기 쉬움

	// Resource Load
	RESOURCE_MANAGER->BeginPlay();
	RESOURCE_MANAGER->Load<Texture>(L"Guitarist", L"..\\Resources\\Images\\Guitarist2.png");

	// Material
	shared_ptr<Material> material = make_shared<Material>();
	material->SetDiffuseMap(RESOURCE_MANAGER->Get<Texture>(L"Guitarist"));
	MaterialDesc& desc = material->GetMaterialDesc();
	desc.ambient = Vec4(1.f);
	desc.diffuse = Vec4(1.f);
	desc.specular = Vec4(1.f);
	shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"03. LightTest.hlsl");
	material->SetShaderInfo(shaderInfo);
	RESOURCE_MANAGER->Add(L"Guitarist", material);

	// Create Actor
	_clientPawn = make_shared<ClientPawn>();
	_clientPawn->Construct();
	

	// Attach Camera
	/*_cameraActor = make_shared<CameraActor>(_clientPawn);
	_cameraActor->Construct();
	_cameraActor->_actorName = "CameraActor";
	_clientPawn->AttachFollowCamera(_cameraActor);*/

	// Create and Set Mesh Info
	shared_ptr<BasicMesh> mesh;
	mesh = RESOURCE_MANAGER->Get<BasicMesh>(L"Cube");
	

	_clientPawn->SetBasicMesh(mesh);
	_clientPawn->SetBasicMaterial(material);

	// Scene에 등장
	SCENE->AddActor(_clientPawn);
}

void CameraDemo::BeginPlay()
{
	SHADER_PARAM_MANAGER->BeginPlay();
	_clientPawn->BeginPlay();
}

void CameraDemo::Tick()
{
	SHADER_PARAM_MANAGER->Update();

	{
		
	}

	_clientPawn->Tick();
}

void CameraDemo::Render()
{
	_clientPawn->Render();
}
