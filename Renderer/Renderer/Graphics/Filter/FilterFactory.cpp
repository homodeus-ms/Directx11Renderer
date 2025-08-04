#include "pch.h"
#include "FilterFactory.h"
#include "Graphics/Filter/Filter.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "BloomPreFilter.h"
#include "CombineFilter.h"
#include "ColorGradingLUTFilter.h"

FilterFactory::~FilterFactory()
{
}

void FilterFactory::CreateFilter(EFilterType type, OUT list<Filter*>& filters)
{
	switch (type)
	{
	case EFilterType::BloomPreFilter: filters.push_back(GetBloomPreFilter()); break;
	case EFilterType::BlurX_Gaussian: filters.push_back(GetBlurX_GaussianFilter()); break;
	case EFilterType::BlurY_Gaussian: filters.push_back(GetBlurY_GaussianFilter()); break;
	case EFilterType::DownSampling: filters.push_back(GetDownSamplingFilter()); break;
	case EFilterType::UpSampling: filters.push_back(GetUpSamplingFilter()); break;
	case EFilterType::LUT_ColorGrading:
	{ 
		filters.push_back(GetLUTFilter()); 
		_onFilterWithBlendFactorCreated.Broadcast(static_cast<ColorGradingLUTFilter*>(filters.back())->GetLUTMixRatioPtr());
		
		break; 
	}
	case EFilterType::Combine: break;
	case EFilterType::Gaussian:
	{
		filters.push_back(GetDownSamplingFilter());

		for (uint32 i = 0; i < GAUSSIAN_REPEAT; ++i)
		{
			filters.push_back(GetBlurX_GaussianFilter());
			filters.push_back(GetBlurY_GaussianFilter());
		}

		filters.push_back(GetUpSamplingFilter());
		filters.push_back(GetCombineFilter());
		break;
	}
	case EFilterType::Bloom:
	{
		Filter* bloomPreFilter = GetBloomPreFilter();
		filters.push_back(bloomPreFilter);

		for (uint32 i = 0; i < GAUSSIAN_REPEAT; ++i)
		{
			filters.push_back(GetBlurX_GaussianFilter());
			filters.push_back(GetBlurY_GaussianFilter());
		}

		filters.push_back(GetUpSamplingFilter());
		
		Filter* combineFilter = GetCombineFilter();
		filters.push_back(combineFilter);

		_onBloomFilterCreated.Broadcast(
			static_cast<BloomPreFilter*>(bloomPreFilter)->GetBrightnessPtr(),
			static_cast<CombineFilter*>(combineFilter)->GetFilterStrengthPtr()
		);

		return;
	}
	default:
	{
		LOG(Log, "No Such Filter Type");
		assert(false);
		break;
	}
	}
}

void FilterFactory::RemoveFilter(list<Filter*> filters)
{
	for (Filter* filter : filters)
		SAFE_DELETE(filter);
}

Filter* FilterFactory::InitCreatedFilter(Filter* newFilter, const wstring& psShaderName, uint32 viewportX, uint32 viewportY)
{
	shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(FILTER_COMMON_VS_PATH, psShaderName);
	newFilter->Initialize(shaderInfo, viewportX, viewportY);
	return newFilter;
}

Filter* FilterFactory::GetCopyFilter()
{
	Filter* filter = new Filter(EFilterType::CopyFilter);
	return InitCreatedFilter(filter, L"Filters/Sampling.hlsl", VIEW_X, VIEW_Y);
}

Filter* FilterFactory::GetBloomPreFilter()
{
	Filter* filter = new BloomPreFilter();
	return InitCreatedFilter(filter, L"Filters/BloomPre.hlsl", VIEW_X / DOWN_SAMPLE_RATIO, VIEW_Y / DOWN_SAMPLE_RATIO);
}

Filter* FilterFactory::GetBlurX_GaussianFilter()
{
	Filter* filter = new Filter(EFilterType::BlurX_Gaussian);
	return InitCreatedFilter(filter, L"Filters/BlurX.hlsl", VIEW_X/ DOWN_SAMPLE_RATIO, VIEW_Y / DOWN_SAMPLE_RATIO);
}

Filter* FilterFactory::GetBlurY_GaussianFilter()
{
	Filter* filter = new Filter(EFilterType::BlurY_Gaussian);
	return InitCreatedFilter(filter, L"Filters/BlurY.hlsl", VIEW_X / DOWN_SAMPLE_RATIO, VIEW_Y / DOWN_SAMPLE_RATIO);;
}

Filter* FilterFactory::GetDownSamplingFilter()
{
	Filter* filter = new Filter(EFilterType::DownSampling);
	return InitCreatedFilter(filter, L"Filters/Sampling.hlsl", VIEW_X / DOWN_SAMPLE_RATIO, VIEW_Y / DOWN_SAMPLE_RATIO);
}

Filter* FilterFactory::GetUpSamplingFilter()
{
	Filter* filter = new Filter(EFilterType::UpSampling);
	return InitCreatedFilter(filter, L"Filters/Sampling.hlsl", VIEW_X, VIEW_Y);
}

Filter* FilterFactory::GetCombineFilter()
{
	Filter* filter = new CombineFilter();
	return InitCreatedFilter(filter, L"Filters/Combine.hlsl", GWinSizeX, GWinSizeY);
}

Filter* FilterFactory::GetLUTFilter()
{
	Filter* filter = new ColorGradingLUTFilter();
	return InitCreatedFilter(filter, L"Filters/ColorGradingLUT.hlsl", GWinSizeX, GWinSizeY);
}


