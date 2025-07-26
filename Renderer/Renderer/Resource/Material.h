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
	MaterialDesc* GetDesc() { return &_desc; }

	shared_ptr<LoadedTexture> GetDiffuseMap() { return _diffuseMap; }
	shared_ptr<LoadedTexture> GetNormalMap() { return _normalMap; }
	shared_ptr<LoadedTexture> GetSpecularMap() { return _specularMap; }
	shared_ptr<ShaderInfo> GetShaderInfo() { return _shaderInfo; }
	const array<SRVBindingInfo, TEXTURE_TYPE_COUNT>& GetSRVBindingInfos() const { return _srvBindingInfos; }

	void SetShaderInfo(shared_ptr<ShaderInfo> shaderInfo) { _shaderInfo = shaderInfo; }
	void SetTextureMap(ETextureType textureType, shared_ptr<LoadedTexture> texture);
	void SetDiffuseMap(shared_ptr<LoadedTexture> diffuseMap);
	void SetNormalMap(shared_ptr<LoadedTexture> normalMap);
	void SetSpecularMap(shared_ptr<LoadedTexture> specularMap);
	 
	shared_ptr<Material> Clone();

private:
	MaterialDesc _desc;

	shared_ptr<LoadedTexture> _diffuseMap; // or CubeMap (When Making CubeMap)
	shared_ptr<LoadedTexture> _normalMap;
	shared_ptr<LoadedTexture> _specularMap;

	// [0] : diffuse, [1] : normal, [2] : specular
	array<SRVBindingInfo, TEXTURE_TYPE_COUNT> _srvBindingInfos;
	
	shared_ptr<ShaderInfo> _shaderInfo = nullptr;
};

