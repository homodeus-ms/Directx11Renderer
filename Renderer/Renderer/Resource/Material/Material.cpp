#include "pch.h"
#include "Material.h"

Material::Material()
{
	_srvBindingInfos.resize(MATERIAL_SRV_COUNT);
}

Material::~Material()
{
}

Material::Material(const Material& other)
	: Super(other)
	, _albedoMap(other._albedoMap)
	, _normalMap(other._normalMap)
	, _heightMap(other._heightMap)
	, _AOMap(other._AOMap)
	, _specularMap(other._specularMap)
	, _metallicRoughnessMap(other._metallicRoughnessMap)
	, _emissiveMap(other._emissiveMap)
{
}

void Material::SetAlbedoMap(shared_ptr<LoadedTexture> albedoMap)
{
	_albedoMap = albedoMap;
	uint8 slot = static_cast<uint8>(EMatTextureType::Albedo);
	AddBindingInfo(_albedoMap, slot, EShaderStage::PsStage);
}

void Material::SetNormalMap(shared_ptr<LoadedTexture> normalMap)
{
	_normalMap = normalMap;
	uint8 slot = static_cast<uint8>(EMatTextureType::Normal);
	AddBindingInfo(_normalMap, slot, EShaderStage::PsStage);
}

void Material::SetHeightMap(shared_ptr<LoadedTexture> heightMap)
{
	_heightMap = heightMap;
	uint8 slot = static_cast<uint8>(EMatTextureType::Height);
	AddBindingInfo(_heightMap, slot, EShaderStage::VsStage);

	_desc.bUseHeightMap = 1;
}

void Material::SetAOMap(shared_ptr<LoadedTexture> AOMap)
{
	_AOMap = AOMap;
	uint8 slot = static_cast<uint8>(EMatTextureType::AO);
	AddBindingInfo(_AOMap, slot, EShaderStage::PsStage);
}

void Material::SetSpecularMap(shared_ptr<LoadedTexture> specularMap)
{
	_specularMap = specularMap;
	uint8 slot = static_cast<uint8>(EMatTextureType::Specular);
	AddBindingInfo(_specularMap, slot, EShaderStage::PsStage);
}

void Material::SetMetallicRoughnessMap(shared_ptr<LoadedTexture> MetallicRoughnessMap)
{
	_metallicRoughnessMap = MetallicRoughnessMap;
	uint8 slot = static_cast<uint8>(EMatTextureType::MetallicRoughness);
	AddBindingInfo(_metallicRoughnessMap, slot, EShaderStage::PsStage);
}

void Material::SetMetallicRoughnessMap(shared_ptr<LoadedTexture> metallic, shared_ptr<LoadedTexture> roughness)
{
	//Utils::CombineMetallicAndRoughness(metallic, roughness);
	//SetMetallicRoughnessMap(metallic);
}

void Material::SetEmissiveMap(shared_ptr<LoadedTexture> emissiveMap)
{
	_emissiveMap = emissiveMap;
	uint8 slot = static_cast<uint8>(EMatTextureType::Emissive);
	AddBindingInfo(_emissiveMap, slot, EShaderStage::PsStage);
}

void Material::SetTextureMap(EMatTextureType type, shared_ptr<LoadedTexture> texture)
{
	switch (type)
	{
	case EMatTextureType::Albedo: SetAlbedoMap(texture); break;
	case EMatTextureType::Normal: SetNormalMap(texture); break;
	case EMatTextureType::Height: SetHeightMap(texture); break;
	case EMatTextureType::AO: SetAOMap(texture); break;
	case EMatTextureType::Specular: SetSpecularMap(texture); break;
	case EMatTextureType::MetallicRoughness: SetMetallicRoughnessMap(texture); break;
	case EMatTextureType::Emissive: SetEmissiveMap(texture); break;
	default: assert(false); break;
	}
}

shared_ptr<MaterialBase> Material::Clone()
{
	return make_shared<Material>(*this);
}
