#pragma once
#include "Resource/ResourceBase.h"
#include "Structs/ShaderParameterTypes.h"
#include "Structs/EShaderStage.h"

struct ShaderInfo;

class MaterialBase : public ResourceBase
{
	using Super = ResourceBase;

public:

	MaterialBase();
	virtual ~MaterialBase();
	MaterialBase(const MaterialBase& other);

	MaterialDesc& GetMaterialDesc() { return _desc; }
	MaterialDesc* GetDesc() { return &_desc; }

	const vector<SRVBindingInfo>& GetSRVBindingInfos() const { return _srvBindingInfos; };
 	bool IsUsingHeightMap() { return _desc.bUseHeightMap == 1; }
	float GetHeightScale() { return _heightScale; }
	uint32* GetUseHeightMapPtr() { return &_desc.bUseHeightMap; }
	float* GetHeightScalePtr() { return &_heightScale; }
	float* GetMetallicValuePtr() { return &_desc.metallic; }
	float* GetRoughnessValuePtr() { return &_desc.roughness; }
	EMaterialType GetMaterialType() { return _desc.MaterialType; }
	float* GetBlendFactorPtr() { return &_blendFactor; }
	float GetBlendFactor() { return _blendFactor; }

	//shared_ptr<ShaderInfo> GetShaderInfo() { return _shaderInfo; }
	//void SetShaderInfo(shared_ptr<ShaderInfo> shaderInfo) { _shaderInfo = shaderInfo; }
	
	virtual void SetAlbedoMap(shared_ptr<LoadedTexture> albedoMap) {}
	virtual void SetNormalMap(shared_ptr<LoadedTexture> normalMap) {}
	virtual void SetHeightMap(shared_ptr<LoadedTexture> heightMap) {}
	virtual void SetAOMap(shared_ptr<LoadedTexture> AOMap) {}
	virtual void SetSpecularMap(shared_ptr<LoadedTexture> specularMap) {}
	virtual void SetMetallicRoughnessMap(shared_ptr<LoadedTexture> metallicMap) {}
	virtual void SetMetallicRoughnessMap(shared_ptr<LoadedTexture> metallic, shared_ptr<LoadedTexture> roughness) {}
	virtual void SetEmissiveMap(shared_ptr<LoadedTexture> emissiveMap) {}
	virtual void SetBRDFMap(shared_ptr<LoadedTexture> brdfMap) {}
	virtual void SetEnvMap(shared_ptr<LoadedTexture> envMap) {}
	virtual void SetIBLSpecularMap(shared_ptr<LoadedTexture> iblSpecularMap) {}
	virtual void SetIBLDiffuseMap(shared_ptr<LoadedTexture> iblDiffuseMap) {}
	void SetBlendFactor(float blendFactor) { _blendFactor = blendFactor; } 
	 
	virtual void SetTextureMap(EMatTextureType type, shared_ptr<LoadedTexture> texture) {}
	virtual void SetIsUsingMetallicRoughnessMap(bool bIsUsing) {}

	virtual shared_ptr<MaterialBase> Clone() abstract;

protected:
	void AddBindingInfo(shared_ptr<LoadedTexture> texture, uint32 slot, EShaderStage stage, uint32 indexOffset = 0);
	MaterialDesc _desc;

	vector<SRVBindingInfo> _srvBindingInfos;
	//shared_ptr<ShaderInfo> _shaderInfo = nullptr;

	// For Control From UI
	float _heightScale = 1.f;
	float _blendFactor = 1.f;
};

