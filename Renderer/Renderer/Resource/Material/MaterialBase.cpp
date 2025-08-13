#include "pch.h"
#include "MaterialBase.h"
#include "Resource/Texture/LoadedTexture.h"

MaterialBase::MaterialBase()
    : Super(EResourceType::Material)
{
}

MaterialBase::~MaterialBase()
{
}

MaterialBase::MaterialBase(const MaterialBase& other)
    : Super(EResourceType::Material)
    , _desc(other._desc)
    , _srvBindingInfos(other._srvBindingInfos)
    , _heightScale(other._heightScale)
{
}

void MaterialBase::AddBindingInfo(shared_ptr<LoadedTexture> texture, uint32 slot, EShaderStage stage, uint32 indexOffset)
{
    SRVBindingInfo info = {
        slot,
        stage,
        texture->GetSRV()
    };

    _srvBindingInfos[slot - indexOffset] = info;
}
