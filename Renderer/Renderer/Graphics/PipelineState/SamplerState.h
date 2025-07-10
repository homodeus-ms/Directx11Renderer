#pragma once

enum class ESamplerAddressMode : uint8
{
	Wrap,
	Mirror,
	Clamp,
	Border,
};

enum class ESamplerFilter : uint8
{
	Point,
	Linear,
	Anisotropic,
	Comparison,
};

enum class EComparisionFunc : uint8
{
	Never,
	Less,
	Equal,
	NotEqual,
	Greater,
	Always,
};

struct SamplerDesc
{
	float MipLODBias = 0.f;
	float MinLOD = FLT_MIN;
	float MaxLOD = FLT_MAX;
	uint32 MaxAnisotropy = 16;
	float BorderColor[4] = { 1, 0, 0, 1 };
	ESamplerAddressMode AddressMode = ESamplerAddressMode::Wrap;
	ESamplerFilter Filter = ESamplerFilter::Linear;
	EComparisionFunc Comparison = EComparisionFunc::Always;
	uint8 Dummy; /* padding for struct align */
};

class SamplerState
{
public:

	SamplerState();
	SamplerState(const SamplerDesc& desc);
	~SamplerState();

	ComPtr<ID3D11SamplerState> GetComPtr() { return _currentState; }
	void ChangeAddressMode(ESamplerAddressMode mode);
private:

	void SetD3DDesc(const SamplerDesc& desc, D3D11_SAMPLER_DESC& OUT d3dDesc);
	void CreateDefaultStates();
	void CreateDefaultWrapMode();
	void CreateDefaultMirrorMode();
	void CreateDefaultClampMode();
	void CreateDefaultBorderMode();
	void CreateState(D3D11_SAMPLER_DESC* descPtr, ComPtr<ID3D11SamplerState> OUT state);
	
	SamplerDesc _defaultDesc{};
	D3D11_SAMPLER_DESC _d3dDefaultDesc{};
	SamplerDesc _desc{};
	D3D11_SAMPLER_DESC _d3dDesc{};

	ComPtr<ID3D11SamplerState> _currentState;
	
	ComPtr<ID3D11SamplerState> _userSetState;
	ComPtr<ID3D11SamplerState> _defaultWrapModeState;
	ComPtr<ID3D11SamplerState> _defaultMirrorModeState;
	ComPtr<ID3D11SamplerState> _defaultClampModeState;
	ComPtr<ID3D11SamplerState> _defaultBorderModeState;
};

inline D3D11_TEXTURE_ADDRESS_MODE ToD3DAddressMode(ESamplerAddressMode mode)
{
	switch (mode)
	{
	case ESamplerAddressMode::Wrap:   return D3D11_TEXTURE_ADDRESS_WRAP;
	case ESamplerAddressMode::Mirror: return D3D11_TEXTURE_ADDRESS_MIRROR;
	case ESamplerAddressMode::Clamp:  return D3D11_TEXTURE_ADDRESS_CLAMP;
	case ESamplerAddressMode::Border: return D3D11_TEXTURE_ADDRESS_BORDER;
	default:                          return D3D11_TEXTURE_ADDRESS_WRAP;
	}
}

inline D3D11_FILTER ToD3DFilter(ESamplerFilter filter)
{
	switch (filter)
	{
	case ESamplerFilter::Point: return D3D11_FILTER_MIN_MAG_MIP_POINT;
	case ESamplerFilter::Linear: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	case ESamplerFilter::Anisotropic: return D3D11_FILTER_ANISOTROPIC;
	case ESamplerFilter::Comparison: return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	default: return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
}

inline D3D11_COMPARISON_FUNC ToD3DComparisonFunc(EComparisionFunc func)
{
	switch (func)
	{
	case EComparisionFunc::Never:     return D3D11_COMPARISON_NEVER;
	case EComparisionFunc::Less:      return D3D11_COMPARISON_LESS;
	case EComparisionFunc::Equal:     return D3D11_COMPARISON_EQUAL;
	case EComparisionFunc::NotEqual:  return D3D11_COMPARISON_NOT_EQUAL;
	case EComparisionFunc::Greater:   return D3D11_COMPARISON_GREATER;
	case EComparisionFunc::Always:    return D3D11_COMPARISON_ALWAYS;
	default:                          return D3D11_COMPARISON_ALWAYS;
	}
}

