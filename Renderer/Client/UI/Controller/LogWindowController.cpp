#include "pch.h"
#include "LogWindowController.h"

LogWindowController::LogWindowController()
{
}

LogWindowController::~LogWindowController()
{
}

void LogWindowController::BeginPlay()
{
	_logs.reserve(200);
	g_OnLogDelegate.BindObject(shared_from_this(), &LogWindowController::LogCallback);
}

void LogWindowController::LogCallback(const string& log)
{
	_logs.push_back(log);
}

void LogWindowController::CreateDebugWindow()
{
	ImGui::SetNextWindowPos({ 0, GUI_MainWindowSizeY }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ GUI_DebugWindowSizeX, GUI_DebugWindowSizeY }, ImGuiCond_Always);
	ImGui::Begin("DebugWindow", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	ImGui::Separator();
	// Begin a child window with scroll functionality
	ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	for (const auto& log : _logs) 
	{
		ImVec4 color = ImVec4(1, 1, 1, 1); // White

		if (log.find("[Error]") != std::string::npos)
			color = ImVec4(1, 0.3f, 0.3f, 1); // Red
		else if (log.find("[Warning]") != std::string::npos)
			color = ImVec4(1, 1, 0.3f, 1);    // Yellow

		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::TextUnformatted(log.c_str());
		ImGui::PopStyleColor();
	}

	// Optional: Auto-scroll to bottom when new logs arrive
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();

	ImGui::End();
}
