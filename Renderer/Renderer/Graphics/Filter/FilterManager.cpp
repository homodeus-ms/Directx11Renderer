#include "pch.h"
#include "FilterManager.h"
#include "FilterFactory.h"
#include "Filter.h"
#include "ColorGradingLUTFilter.h"
#include "Graphics/PipelineState/PipelineState.h"


FilterManager::~FilterManager()
{
	if (_filterFactory)
		_filterFactory->RemoveFilter(_filters);
}

void FilterManager::Construct()
{
	_filterFactory = GET_SINGLE(FilterFactory);
	_filterStates = PipelineState::GetFilterStates();
}

void FilterManager::AddFilter(EFilterType type) 
{
	if (_activateFilters.contains(type))
		return;

	// TEMP : 현재 UI에서 적용할 수 있는 필터는 2가지
	if (!(type == EFilterType::Bloom || type == EFilterType::LUT_ColorGrading))
		return;

	if (_filters.empty())
	{
		_filterFactory->CreateFilter(type, _filters);
		_activateFilters.insert(type);
		return;
	}

	// Filter 적용되는 순서를 따져서 list에 배치, 현재는 bloom과 LUT 두개뿐 ( bloom - LUT 순서)
	if (_activateFilters.contains(EFilterType::Bloom))
	{
		_filters.back()->UseSelfRenderTarget();

		list<Filter*> LUTFilter;
		_filterFactory->CreateFilter(EFilterType::LUT_ColorGrading, LUTFilter);
		_filters.splice(_filters.end(), LUTFilter);
	}
	else if (_activateFilters.contains(EFilterType::LUT_ColorGrading))
	{
		list<Filter*> bloomFilter;
		_filterFactory->CreateFilter(EFilterType::Bloom, bloomFilter);
		bloomFilter.splice(bloomFilter.end(), _filters);
		_filters.swap(bloomFilter);
	}

	_activateFilters.insert(type);
}

void FilterManager::RemoveFilter(EFilterType type)
{
	if (!_activateFilters.contains(type))
		return;

	_activateFilters.erase(type);

	if (_activateFilters.empty())
	{
		_filters.clear();
		return;
	}

	// 현재 동시 활성화 될 수 있는 필터는 2가지, 2개가 다 활성화 되어 있는 상황
	// 그러면 LUT필터는 back()에 있음
	if (type == EFilterType::Bloom)
	{
		list<Filter*> temp;
		temp.push_back(_filters.back());
		_filters.pop_back();
		temp.push_front(_filters.back());
		_filters.pop_back();

		_removableFilters.swap(_filters);
		_filters.splice(_filters.end(), temp);
	}
	else if (type == EFilterType::LUT_ColorGrading)
	{
		// LUT, Combine
		_removableFilters.push_back(_filters.back());
		_filters.pop_back();
		_removableFilters.push_back(_filters.back());
		_filters.pop_back();
	}

	SendRemovableFilters();
}

void FilterManager::SetLUTType(const wstring& LUTName)
{
	Filter* filter = nullptr;
	for (Filter* f : _filters)
	{
		if (f->GetFilterType() == EFilterType::LUT_ColorGrading)
		{
			filter = f;
			break;
		}
	}
	if (filter == nullptr)
	{
		LOG(Log, "No LUTFilter in filters");
		return;
	}
	
	static_cast<ColorGradingLUTFilter*>(filter)->SetNewLUT(LUTName);
}

void FilterManager::SendRemovableFilters()
{
	_filterFactory->RemoveFilter(_removableFilters);
	_removableFilters.clear();
}

void FilterManager::RenderFilters()
{
	if (_filters.empty() || !_bFilterOn)
		return;

	CONTEXT->IASetPrimitiveTopology(_filterStates->_topology);
	CONTEXT->RSSetState(_filterStates->_rsState.Get());
	CONTEXT->PSSetSamplers(0, 1, _filterStates->_samplerState.GetAddressOf());

	ComPtr<ID3D11ShaderResourceView> inputSRV = GRAPHICS->GetBackBufferSRV();

	Filter* startFilter = _filters.front();
	startFilter->SetShaderResources({ inputSRV });
	startFilter->Render();
	Filter* lastCombineFilter = _filters.back();
	lastCombineFilter->SetRenderTargets({ GRAPHICS->GetBackBufferRTV() });

	for (auto it = std::next(_filters.begin()); it != _filters.end(); ++it)
	{
		Filter* filter = *it;

		filter->Tick();

		Filter* prevFilter = *std::prev(it);

		if (filter->GetFilterType() == EFilterType::Combine)
		{
			filter->SetShaderResources({ prevFilter->GetSRV(), inputSRV });
			inputSRV = filter->GetSRV();
		}
		else
		{
			filter->SetShaderResources({ prevFilter->GetSRV() });
		}

		filter->Render();
	}
}