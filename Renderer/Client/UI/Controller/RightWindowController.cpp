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
	ImGui::SetNextWindowPos({ GWinSizeX - GUI_MainWindowSizeX, 0 }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ GUI_MainWindowSizeX, GUI_MainWindowSizeY }, ImGuiCond_Always);
	ImGui::Begin("RightWindow", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove);

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
