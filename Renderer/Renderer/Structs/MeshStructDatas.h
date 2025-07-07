#pragma once
#include <memory>

class Mesh;
class Texture;
struct ShaderInfos;

struct StaticMeshInfo
{
public:
	

	std::shared_ptr<Mesh> mesh{};
	std::shared_ptr<Texture> texture{};
	std::shared_ptr<ShaderInfos> shaderInfos{};
};

