#include "pch.h"
#include "Material.h"
#include "Resource/Texture/LoadedTexture.h"

Material::Material()
    : Super(EResourceType::Material)
{
}

Material::~Material()
{
}

void Material::SetTextureMap(ETextureType textureType, shared_ptr<LoadedTexture> texture)
{
    switch (textureType)
    {
    case ETextureType::Diffuse:
        SetDiffuseMap(texture);
        break;
    case ETextureType::Normal:
        SetNormalMap(texture);
        break;
    case ETextureType::Specular:
        SetSpecularMap(texture);
        break;
    default:
        LOG(Log, "Not Supported Texture");
        break;
    }
}

void Material::SetDiffuseMap(shared_ptr<LoadedTexture> diffuseMap)
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

void Material::SetNormalMap(shared_ptr<LoadedTexture> normalMap)
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

void Material::SetSpecularMap(shared_ptr<LoadedTexture> specularMap)
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

shared_ptr<Material> Material::Clone()
{
    shared_ptr<Material> material = make_shared<Material>();

    material->_desc = _desc;
    material->_diffuseMap = _diffuseMap;
    material->_normalMap = _normalMap;
    material->_specularMap = _specularMap;
    material->_srvBindingInfos = _srvBindingInfos;
    material->_shaderInfo = _shaderInfo;

    return material;
}
