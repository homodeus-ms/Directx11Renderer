#include "pch.h"
#include "GUIController.h"
#include "LeftWindowController.h"
#include "LogWindowController.h"

GUIController::GUIController()
{
	_leftWindowController = make_shared<LeftWindowController>();
	_logWindowController = make_shared<LogWindowController>();
}

GUIController::~GUIController()
{
	
}

void GUIController::BeginPlay()
{
	GUI->_onTick.BindObject(shared_from_this(), &GUIController::Tick);

	assert(_leftWindowController != nullptr);
	assert(_logWindowController != nullptr);
	_leftWindowController->BeginPlay();
	_logWindowController->BeginPlay();
}

void GUIController::Tick()
{
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	_leftWindowController->CreateLeftWindow();
	_logWindowController->CreateDebugWindow();
	//CreateRightWindow();
	
}

void GUIController::CreateRightWindow()
{
	ImGui::SetNextWindowPos({ GWinSizeX - GUI_MainWindowSizeX, 0 }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ GUI_MainWindowSizeX, GUI_MainWindowSizeY }, ImGuiCond_Always);
	ImGui::Begin("RightWindow", nullptr,
		ImGuiWindowFlags_NoMove);

	ImGui::Text("LightWindow");

	ImGui::End();
}


void GUIController::Test()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// Rookiss
	{
		ImGui::Begin("Rookiss", nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoMove);

		ImGui::Text("Rookiss Hello ImGui!");

		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}


