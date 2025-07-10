#pragma once

constexpr int32 GWinSizeX = 800;
constexpr int32 GWinSizeY = 600;

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
#define SCENE_MANAGER		GET_SINGLE(SceneManager)
#define SCENE				SCENE_MANAGER->GetCurrentScene()

#define DEVICE				GRAPHICS->GetDevice()
#define CONTEXT				GRAPHICS->GetDeviceContext()
#define DT					TIME_MANAGER->GetDeltaTime()
