#pragma once
#include "Utils/Delegate.h"

DECLARE_MULTICAST_DELEGATE(OnTick);

class ImGuiManager
{
	DECLARE_SINGLE(ImGuiManager);

public:
	void BeginPlay();
	void Tick();
	void Render();

	OnTick _onTick;


private:
	
	
};

