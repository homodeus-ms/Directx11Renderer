#pragma once

constexpr uint32 GWinSizeX = 1500;
constexpr uint32 GWinSizeY = 675;
constexpr uint32 GWinStartX = 0;
constexpr uint32 GWinStartY = 0;

constexpr uint32 GUI_LeftWindowSizeX = 300;
constexpr uint32 GUI_LeftWindowSizeY = 600;

constexpr uint32 GViewportStartX = GUI_LeftWindowSizeX;
constexpr uint32 GViewportStartY = 0;

constexpr uint32 GUI_RightWindowSizeX = 300;
constexpr uint32 GUI_RightWindowSizeY = 600;
constexpr uint32 GUI_DebugWindowSizeX = GWinSizeX;
constexpr uint32 GUI_DebugWindowSizeY = 120;

constexpr uint32 SubWindowX = 300;
constexpr uint32 SubWindowY = 500;

enum { VIEW_X = GWinSizeX - GUI_LeftWindowSizeX, VIEW_Y = GWinSizeY };

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