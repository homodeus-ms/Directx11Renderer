#pragma once
#include "ResourceBase.h"

class Texture : public ResourceBase
{
	using Super = ResourceBase;

public:
	Texture();
	~Texture();

	ComPtr<ID3D11ShaderResourceView> GetSRV() const { return _shaderResourceView; }
	ID3D11ShaderResourceView* const* GetSRVAddress() const { return _shaderResourceView.GetAddressOf(); }
	virtual void Load(const wstring& path) override;

	Vec2 GetSize() const { return _size; }

	const DirectX::ScratchImage& GetInfo() const { return _img; }

private:

	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
	Vec2 _size = { 0.f, 0.f };
	DirectX::ScratchImage _img = {};
};

