#pragma once

#pragma comment(lib, "Renderer/Renderer.lib")
#include "Headers/pch.h"

#include "Managers/SceneManager.h"
#include "Managers/ShaderParameterManager.h"
#include "ClientManager/EngineClientFlowManager.h"

#define FLOW_MANAGER GET_SINGLE(EngineClientFlowManager)