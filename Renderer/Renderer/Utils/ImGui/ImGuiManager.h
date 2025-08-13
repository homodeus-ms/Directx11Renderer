#pragma once
#include "Utils/Delegate.h"

DECLARE_MULTICAST_DELEGATE(OnTick);
DECLARE_MULTICAST_DELEGATE(OnSubWindowTick);
DECLARE_MULTICAST_DELEGATE(OnSubWindowHidden);

class ImGuiManager
{
	DECLARE_SINGLE(ImGuiManager);

public:
	void BeginPlay();
	void Tick();
	void Render();

	OnTick _onTick;
	OnSubWindowTick _onSubWindowTick;
	OnSubWindowHidden _onSubWindowHidden;

	void CreateSubContext();
	void ShowSubWindow();
	void HideSubWindow();
	void QuitSubContext();
	void BroadcastSubWindowHidden();

private:
	ImGuiContext* _mainContext = nullptr;
	ImGuiContext* _subContext = nullptr;
	HWND _mainHwnd = NULL;
	HWND _subHwnd = NULL;
	bool _bShowSubWindow = false;
};

