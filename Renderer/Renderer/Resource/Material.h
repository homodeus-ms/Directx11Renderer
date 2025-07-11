#pragma once
#include "ResourceBase.h"
#include "Structs/ShaderParameterTypes.h"

struct ShaderInfo;

class Material : public ResourceBase
{
	using Super = ResourceBase;

public:

	Material();
	virtual ~Material();

	MaterialDesc& GetMaterialDesc() { return _desc; }

	shared_ptr<Texture> GetDiffuseMap() { return _diffuseMap; }
	shared_ptr<Texture> GetNormalMap() { return _normalMap; }
	shared_ptr<Texture> GetSpecularMap() { return _specularMap; }
	const array<SRVBindingInfo, TEXTURE_TYPE_COUNT>& GetSRVBindingInfos() const { return _srvBindingInfos; }

	void SetShaderInfo(shared_ptr<ShaderInfo> shaderInfo) { _shaderInfo = shaderInfo; }
	void SetDiffuseMap(shared_ptr<Texture> diffuseMap);
	void SetNormalMap(shared_ptr<Texture> normalMap);
	void SetSpecularMap(shared_ptr<Texture> specularMap);

	void Update();

	shared_ptr<Material> Clone();

private:
	friend class RenderComponent;

	MaterialDesc _desc;

	shared_ptr<Texture> _diffuseMap;
	shared_ptr<Texture> _normalMap;
	shared_ptr<Texture> _specularMap;

	// [0] : diffuse, [1] : normal, [2] : specular
	array<SRVBindingInfo, TEXTURE_TYPE_COUNT> _srvBindingInfos;
	
	shared_ptr<ShaderInfo> _shaderInfo = nullptr;
};

