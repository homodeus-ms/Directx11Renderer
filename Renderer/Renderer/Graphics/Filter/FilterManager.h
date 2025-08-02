#pragma once
#include "Structs/EFilterType.h"
#include <list>

class Filter;
class FilterFactory;
struct PipelineState;

#define FILTER_MANAGER GET_SINGLE(FilterManager)

class FilterManager
{
	DECLARE_SINGLE(FilterManager);

public:
	virtual ~FilterManager();

	void Construct();
	void RenderFilters();
	void AddFilter(EFilterType type);
	void RemoveFilter(EFilterType type);
	void SetFilterOnOff(bool bFilterOn) { _bFilterOn = bFilterOn; }
	void SetLUTType(const wstring& LUTName);


private:
	void SendRemovableFilters();

	FilterFactory* _filterFactory = nullptr;
	unordered_set<EFilterType> _activateFilters;
	list<Filter*> _filters;
	list<Filter*> _removableFilters;

	//Filter* _finalFilter = nullptr;
	PipelineState* _filterStates{};
	bool _bFilterOn = true;
};

