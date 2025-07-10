#include "pch.h"
#include "SamplerState.h"

SamplerState::SamplerState()
{
	SetD3DDesc(_defaultDesc, _d3dDefaultDesc);
	CreateDefaultStates();
	ChangeAddressMode(ESamplerAddressMode::Wrap);
}

SamplerState::SamplerState(const SamplerDesc& desc)
	: _desc(desc)
{
	SetD3DDesc(_defaultDesc, _d3dDefaultDesc);
	SetD3DDesc(_desc, _d3dDesc);
	CreateDefaultStates();
	CreateState(&_d3dDesc, _userSetState);
	_currentState = _userSetState;
}

SamplerState::~SamplerState()
{
}

void SamplerState::ChangeAddressMode(ESamplerAddressMode mode)
{
	switch (mode)
	{
	case ESamplerAddressMode::Wrap: 
		_currentState = _defaultWrapModeState; 
		break;
	case ESamplerAddressMode::Mirror: 
		_currentState = _defaultMirrorModeState; 
		break;
	case ESamplerAddressMode::Clamp: 
		_currentState = _defaultClampModeState; 
		break;
	case ESamplerAddressMode::Border: 
		_currentState = _defaultBorderModeState; 
		break;
	default:
		break;
	}
}

void SamplerState::CreateDefaultStates()
{
	void CreateDefaultWrapMode();
	void CreateDefaultMirrorMode();
	void CreateDefaultClampMode();
	void CreateDefaultBorderMode();
}

void SamplerState::SetD3DDesc(const SamplerDesc& desc, D3D11_SAMPLER_DESC& OUT d3dDesc)
{
	d3dDesc.MipLODBias = desc.MipLODBias;
	d3dDesc.MinLOD = desc.MinLOD;
	d3dDesc.MaxLOD = desc.MaxLOD;
	d3dDesc.MaxAnisotropy = desc.MaxAnisotropy;
	d3dDesc.BorderColor[0] = desc.BorderColor[0];
	d3dDesc.BorderColor[1] = desc.BorderColor[1];
	d3dDesc.BorderColor[2] = desc.BorderColor[2];
	d3dDesc.BorderColor[3] = desc.BorderColor[3];
	
	d3dDesc.AddressU = ToD3DAddressMode(desc.AddressMode);
	d3dDesc.AddressV = ToD3DAddressMode(desc.AddressMode);
	d3dDesc.AddressW = ToD3DAddressMode(desc.AddressMode);
	d3dDesc.Filter = ToD3DFilter(desc.Filter);
	d3dDesc.ComparisonFunc = ToD3DComparisonFunc(desc.Comparison);
}



void SamplerState::CreateDefaultWrapMode()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MinLOD = FLT_MIN;
	desc.MaxLOD = FLT_MAX;
	desc.MaxAnisotropy = 16;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	
	CreateState(&desc, _defaultWrapModeState);

}

void SamplerState::CreateDefaultMirrorMode()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	desc.MinLOD = FLT_MIN;
	desc.MaxLOD = FLT_MAX;
	desc.MaxAnisotropy = 16;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

	CreateState(&desc, _defaultMirrorModeState);

}

void SamplerState::CreateDefaultClampMode()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MinLOD = FLT_MIN;
	desc.MaxLOD = FLT_MAX;
	desc.MaxAnisotropy = 16;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

	CreateState(&desc, _defaultClampModeState);

}

void SamplerState::CreateDefaultBorderMode()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;
	desc.MinLOD = FLT_MIN;
	desc.MaxLOD = FLT_MAX;
	desc.MaxAnisotropy = 16;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	
	CreateState(&desc, _defaultBorderModeState);
}

void SamplerState::CreateState(D3D11_SAMPLER_DESC* descPtr, ComPtr<ID3D11SamplerState> OUT state)
{
	HRESULT hr = DEVICE->CreateSamplerState(descPtr, state.GetAddressOf());
	check(hr);
}
