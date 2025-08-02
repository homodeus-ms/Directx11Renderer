#include "pch.h"
#include "RightWindowController.h"

RightWindowController::RightWindowController()
{
}

RightWindowController::~RightWindowController()
{
}

void RightWindowController::BeginPlay()
{
	_currCamera = SCENE->GetCurrCamera();
}

void RightWindowController::CreateRightWindow()
{
	ImGui::SetNextWindowPos({ GWinSizeX - GUI_RightWindowSizeX, 0 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ GUI_RightWindowSizeX, GUI_RightWindowSizeY }, ImGuiCond_Once);

	ImGui::Begin("RightWindow", nullptr);

	DrawCameraSettingWidget();

	ImGui::End();
}

void RightWindowController::DrawCameraSettingWidget()
{
	ImGui::Spacing();
	ImGui::Separator();
	
	ImGui::PushItemWidth(150);
	if (ImGui::Combo("  Camera Selected", &_selectedCameraIndex, CAMERA_LIST, IM_ARRAYSIZE(CAMERA_LIST))) 
	{
		if (_selectedCameraIndex == 0)
			_currCamera = SCENE->SwitchCameraAndGet(ECameraType::MainCamera);
		else
			_currCamera = SCENE->SwitchCameraAndGet(ECameraType::TopViewCamera);
	}

}
