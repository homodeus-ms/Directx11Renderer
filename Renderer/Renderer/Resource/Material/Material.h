#pragma once
#include "MaterialBase.h"

class Material : public MaterialBase
{
	using Super = MaterialBase;

public:
	Material();
	virtual ~Material();
	Material(const Material& other);

	virtual void SetAlbedoMap(shared_ptr<LoadedTexture> albedoMap) override;
	virtual void SetNormalMap(shared_ptr<LoadedTexture> normalMap) override;
	virtual void SetHeightMap(shared_ptr<LoadedTexture> heightMap) override;
	virtual void SetAOMap(shared_ptr<LoadedTexture> AOMap) override;
	virtual void SetSpecularMap(shared_ptr<LoadedTexture> specularMap) override;
	virtual void SetMetallicRoughnessMap(shared_ptr<LoadedTexture> MetallicRoughnessMap) override;
	virtual void SetMetallicRoughnessMap(shared_ptr<LoadedTexture> metallic, shared_ptr<LoadedTexture> roughness) override;
	virtual void SetEmissiveMap(shared_ptr<LoadedTexture> emissiveMap) override;
	
	virtual void SetTextureMap(EMatTextureType type, shared_ptr<LoadedTexture> texture) override;
	
	virtual shared_ptr<MaterialBase> Clone() override;

private:
	shared_ptr<LoadedTexture> _albedoMap;
	shared_ptr<LoadedTexture> _normalMap;
	shared_ptr<LoadedTexture> _heightMap;
	shared_ptr<LoadedTexture> _AOMap;
	shared_ptr<LoadedTexture> _specularMap;
	shared_ptr<LoadedTexture> _metallicRoughnessMap;
	shared_ptr<LoadedTexture> _emissiveMap;

	enum { MATERIAL_SRV_COUNT = 8 };

};

