#include "pch.h"
#include "CombineFilter.h"

CombineFilter::CombineFilter()
	: Super(EFilterType::Combine)
{
}

CombineFilter::~CombineFilter()
{
}

void CombineFilter::Tick()
{
	_filterData.strength = _filterStrength;
}
