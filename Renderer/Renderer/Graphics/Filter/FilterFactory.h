#pragma once
#include "Structs/EFilterType.h"

class Filter;

DECLARE_MULTICAST_DELEGATE_TwoParams(OnBloomFilterCreatedDelegate, float* /* BloomRange */, float* /* FilterStrength*/);
DECLARE_MULTICAST_DELEGATE_OneParam(OnCombineFilterCreatedDelegate, float* /* FilterStrength */);

class FilterFactory
{
	DECLARE_SINGLE(FilterFactory);

public:
	~FilterFactory();
	void CreateFilter(EFilterType type, OUT list<Filter*>& filters);
	void RemoveFilter(list<Filter*> filters);

	//OnBloomPreFilterCreatedDelegate& GetBloomPreFilterCreatedDelegate() { return _onBloomPreFilterCreated; }
	//OnCombineFilterCreatedDelegate& GetCombineFilterCreatedDelegate() { return _onCombineFilterCreated; }
	OnBloomFilterCreatedDelegate _onBloomFilterCreated;
	OnCombineFilterCreatedDelegate _onCombineFilterCreated;
	
	Filter* InitCreatedFilter(Filter* newFilter, const wstring& psShaderName, uint32 viewportX = VIEW_X, uint32 viewportY = VIEW_Y);
	Filter* GetCopyFilter();
	Filter* GetBloomPreFilter();
	Filter* GetBlurX_GaussianFilter();
	Filter* GetBlurY_GaussianFilter();
	Filter* GetDownSamplingFilter();
	Filter* GetUpSamplingFilter();
	Filter* GetCombineFilter();
	Filter* GetLUTFilter();

	const wstring FILTER_COMMON_VS_PATH = L"Filters/FilterCommonVS.hlsl";
	const uint32 DOWN_SAMPLE_RATIO = 16;
	const uint32 GAUSSIAN_REPEAT = 5;
	
};

