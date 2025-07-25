#pragma once

constexpr uint32 GWinSizeX = 1500;
constexpr uint32 GWinSizeY = 700;
constexpr uint32 GWinStartX = 10;
constexpr uint32 GWinStartY = 10;
// GUI
constexpr uint32 GUI_AreaX = 700;
constexpr uint32 GUI_AreaY = 150;
constexpr uint32 GUI_MainWindowSizeX = 350;
constexpr uint32 GUI_MainWindowSizeY = 550;
constexpr uint32 GUI_DebugWindowSizeX = 1500;
constexpr uint32 GUI_DebugWindowSizeY = 150;

enum { VIEW_X = GWinSizeX - GUI_AreaX, VIEW_Y = GWinSizeY - GUI_AreaY };

#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

#define GET_SINGLE(classname)	classname::GetInstance()


#define check(p)			assert(SUCCEEDED(p))

#define RENDERER			GET_SINGLE(Renderer)	
#define INPUT_MANAGER		GET_SINGLE(InputManager)
#define TIME_MANAGER		GET_SINGLE(TimeManager)
#define RESOURCE_MANAGER	GET_SINGLE(ResourceManager)
#define GRAPHICS			GET_SINGLE(Graphics)
#define GUI                 GET_SINGLE(ImGuiManager)
#define NAME_MANAGER        (RENDERER->GetNamaManager())

#define DEVICE				GRAPHICS->GetDevice()
#define CONTEXT				GRAPHICS->GetDeviceContext()
#define DT					TIME_MANAGER->GetDeltaTime()

#define SAFE_DELETE(ptr)		\
if (ptr)						\
{								\
    delete ptr;					\
	ptr = nullptr;				\
}								