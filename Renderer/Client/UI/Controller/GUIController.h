#pragma once

class LeftWindowController;
class LogWindowController;

class GUIController : public enable_shared_from_this<GUIController>
{
public:

	GUIController();
	virtual ~GUIController();

	virtual void BeginPlay();
	virtual void Tick();

	void Test();

protected:

	shared_ptr<LeftWindowController> _leftWindowController;
	shared_ptr<LogWindowController> _logWindowController;
	// Right
	void CreateRightWindow();

	bool show_demo_window = true;
	bool show_another_window = false;
	Vec4 clear_color = Vec4(0.5f);
};

