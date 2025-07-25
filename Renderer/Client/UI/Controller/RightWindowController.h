#pragma once

class CameraActor;

class RightWindowController : public enable_shared_from_this<RightWindowController>
{
public:
	RightWindowController();
	virtual ~RightWindowController();

	void BeginPlay();

	void CreateRightWindow();

private:
	void DrawCameraSettingWidget();

	// Camera
	const char* CAMERA_LIST[2] = { "Main Camera", "TopView Camera" };
	int32 _selectedCameraIndex = 0;
	shared_ptr<CameraActor> _currCamera;
	
};

