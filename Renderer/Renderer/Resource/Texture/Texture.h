#pragma once
#include "Resource/ResourceBase.h"

struct SRVBindingInfo;

class Texture : public ResourceBase
{
	using Super = ResourceBase;

public:
	Texture();
	virtual ~Texture();

	ComPtr<ID3D11ShaderResourceView> GetSRV() const { return _SRV; }
	ID3D11ShaderResourceView* const* GetSRVAddress() const { return _SRV.GetAddressOf(); }
	shared_ptr<SRVBindingInfo> GetSRVBindingInfo() const { return _srvBindingInfo; }

protected:
	ComPtr<ID3D11ShaderResourceView> _SRV;
	shared_ptr<SRVBindingInfo> _srvBindingInfo;
};

