#include "pch.h"
#include "ColorGradingLUTFilter.h"
#include "Resource/LUT.h"

ColorGradingLUTFilter::ColorGradingLUTFilter()
	: Super(EFilterType::LUT_ColorGrading)
{
	
}

ColorGradingLUTFilter::~ColorGradingLUTFilter()
{
}

void ColorGradingLUTFilter::Tick()
{
	_filterData.strength = _LutMixRatio;
}

void ColorGradingLUTFilter::SetNewLUT(const wstring& newLUTName)
{
	if (_LUTName == newLUTName)
		return;

	shared_ptr<LUT> lut = RESOURCE_MANAGER->Get<LUT>(newLUTName);
	if (lut == nullptr)
		return;

	GetLUT(newLUTName);
}

void ColorGradingLUTFilter::GetLUT(const wstring& LUTName)
{
	_LUT = RESOURCE_MANAGER->Get<LUT>(LUTName);
	if (_LUT == nullptr)
	{
		LOG(Warning, "NOT Exist such a name of LUT Resource");
		return;
	}
	shared_ptr<SRVBindingInfo> info = _LUT->GetSRVBindingInfo();
	CONTEXT->PSSetShaderResources(info->slot, 1, info->srv.GetAddressOf());
}
