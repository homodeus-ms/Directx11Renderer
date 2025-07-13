#pragma once

class ImGuiManager
{
	DECLARE_SINGLE(ImGuiManager);

public:
	void BeginPlay();
	void Tick();
	void Render();

	void Test();

	void ShowDefaultGUI();

private:
	bool show_demo_window = true;
	bool show_another_window = true;

	Vec4 clear_color = Vec4(0.5f);
};

