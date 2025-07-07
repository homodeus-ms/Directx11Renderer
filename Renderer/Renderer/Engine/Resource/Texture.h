#pragma once
#include "ResourceBase.h"

class Texture : public ResourceBase
{
	using Super = ResourceBase;

public:
	Texture();
	~Texture();

	ComPtr<ID3D11ShaderResourceView> GetComPtr() const { return _shaderResourveView; }

	virtual void Load(const wstring& path) override;

	Vec2 GetSize() const { return _size; }

	const DirectX::ScratchImage& GetInfo() const { return _img; }

private:

	ComPtr<ID3D11ShaderResourceView> _shaderResourveView;
	Vec2 _size = { 0.f, 0.f };
	DirectX::ScratchImage _img = {};
};

