#include "pch.h"
#include "ToneMappingFilter.h"

ToneMappingFilter::ToneMappingFilter()
	: Super(EFilterType::ToneMapping)
{
}

ToneMappingFilter::~ToneMappingFilter()
{
}

void ToneMappingFilter::Tick()
{
	// TEMP : options.x - Exposure, options.y = Gamma 
	_filterData.options.x = _exposure;
	_filterData.options.y = _gamma;
}
