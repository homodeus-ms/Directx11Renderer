#include "pch.h"
#include "Material.h"
#include "Resource/Texture.h"

Material::Material()
    : Super(ResourceType::Material)
{
}

Material::~Material()
{
}

void Material::SetDiffuseMap(shared_ptr<Texture> diffuseMap)
{
    _diffuseMap = diffuseMap;
    uint8 slot = static_cast<uint8>(ETextureType::Diffuse);

    _srvBindingInfos[slot] =
    {
        slot,
        EShaderStage::PsStage,
        _diffuseMap->GetSRV()
    };
}

void Material::SetNormalMap(shared_ptr<Texture> normalMap)
{
    _normalMap = normalMap;
    uint8 slot = static_cast<uint8>(ETextureType::Normal);

    _srvBindingInfos[slot] =
    {
        slot,
        EShaderStage::PsStage,
        _normalMap->GetSRV()
    };
}

void Material::SetSpecularMap(shared_ptr<Texture> specularMap)
{
    _specularMap = specularMap;
    uint8 slot = static_cast<uint8>(ETextureType::Specular);

    _srvBindingInfos[slot] =
    {
        slot,
        EShaderStage::PsStage,
        _specularMap->GetSRV()
    };
}

void Material::Update()
{
}

shared_ptr<Material> Material::Clone()
{
    return shared_ptr<Material>();
}
