#include "pch.h"
#include "BloomPreFilter.h"

BloomPreFilter::BloomPreFilter()
	: Super(EFilterType::BloomPreFilter)
{
	
}

BloomPreFilter::~BloomPreFilter()
{
}

void BloomPreFilter::Tick()
{
	_filterData.brightnessThreshold = _brightnessThreshold;
}
