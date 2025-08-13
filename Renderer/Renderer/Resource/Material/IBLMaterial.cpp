#include "pch.h"
#include "IBLMaterial.h"

IBLMaterial::IBLMaterial()
{
	_srvBindingInfos.resize(IBL_MATERIAL_SRV_COUNT);
}

IBLMaterial::~IBLMaterial()
{
}

IBLMaterial::IBLMaterial(const IBLMaterial& other)
	: Super(other)
	, _envMap(other._envMap)
	, _iblSpecularMap(other._iblSpecularMap)
	, _iblDiffuseMap(other._iblDiffuseMap)
	, _brdfMap(other._brdfMap)
{
}

void IBLMaterial::SetEnvMap(shared_ptr<LoadedTexture> envMap)
{
	_envMap = envMap;
	uint8 slot = static_cast<uint8>(EIBLTextureType::Env);
	AddBindingInfo(_envMap, slot, EShaderStage::PsStage, MAT_TEXTURE_TYPE_COUNT);
}

void IBLMaterial::SetIBLSpecularMap(shared_ptr<LoadedTexture> iblSpecularMap)
{
	_iblSpecularMap = iblSpecularMap;
	uint8 slot = static_cast<uint8>(EIBLTextureType::IBL_Spec);
	AddBindingInfo(_iblSpecularMap, slot, EShaderStage::PsStage, MAT_TEXTURE_TYPE_COUNT);
}

void IBLMaterial::SetIBLDiffuseMap(shared_ptr<LoadedTexture> iblDiffuseMap)
{
	_iblDiffuseMap = iblDiffuseMap;
	uint8 slot = static_cast<uint8>(EIBLTextureType::IBL_Diff);
	AddBindingInfo(_iblDiffuseMap, slot, EShaderStage::PsStage, MAT_TEXTURE_TYPE_COUNT);
}

void IBLMaterial::SetBRDFMap(shared_ptr<LoadedTexture> brdfMap)
{
	_brdfMap = brdfMap;
	uint8 slot = static_cast<uint8>(EIBLTextureType::IBL_BRDF);
	AddBindingInfo(_brdfMap, slot, EShaderStage::PsStage, MAT_TEXTURE_TYPE_COUNT);
}

shared_ptr<MaterialBase> IBLMaterial::Clone()
{
	return make_shared<IBLMaterial>(*this);
}
