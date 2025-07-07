#include "pch.h"
#include "ResourceManager.h"
#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Mesh.h"
#include "Graphics/Shader/Shader.h"

void ResourceManager::BeginPlay()
{
	CreateDefaultMesh();
}

void ResourceManager::CreateDefaultMesh()
{
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateQuad();
		Add(L"Quad", mesh);
	}
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateCube();
		Add(L"Cube", mesh);
	}
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateSphere();
		Add(L"Sphere", mesh);
	}
}
