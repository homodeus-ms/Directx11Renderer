#include "pch.h"
#include "ShadowMap.h"
#include "Actor/Actor.h"
#include "Actor/LightActor.h"
#include "Managers/ShaderParameterManager.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Buffer/VertexData.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/GeometryShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Resource/Texture/LoadedTexture.h"
#include "Resource/Texture/DepthMapTexture.h"
#include "Resource/Texture/CubeDepthMapTexture.h"
#include "Components/Transform.h"
#include "Managers/RenderManager.h"
#include "Graphics/RenderPass/CommonRenderResource.h"
#include "Graphics/RenderPass/PSO.h"
#include "Graphics/PipelineState/PipelineState.h"

ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
	for (int32 i = 0; i < MAX_SHADOW_MAP_COUNT; ++i)
	{
		SAFE_DELETE(_shadowTextures[i])
	}

	SAFE_DELETE(_shadowCubeTexture);
	SAFE_DELETE(_depthMapTexture);
}

void ShadowMap::Construct()
{
	CreateShadowTexture();
	SetShadowViewport();
	_shadowPSO = GET_SINGLE(CommonRenderResource)->_shadowPSO;
	_shadowPointLightPSO = GET_SINGLE(CommonRenderResource)->_shadowPointLightPSO;
	_depthMapPSO = GET_SINGLE(CommonRenderResource)->_getDepthMapPSO;
}

shared_ptr<SRVBindingInfo> ShadowMap::DrawDepthMap(const vector<shared_ptr<Actor>>& actors)
{
	ComPtr<ID3D11DepthStencilView> dsv = _depthMapTexture->GetDSV();
	CONTEXT->OMSetRenderTargets(0, nullptr, dsv.Get());
	CONTEXT->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	CONTEXT->RSSetViewports(1, &_shadowViewport);

	// 모든 물체의 depth 기록
	for (shared_ptr<Actor> actor : actors)
	{
		actor->Render();
	}

	return _depthMapTexture->GetSRVBindingInfo();
}

void ShadowMap::CreateAndDrawShadowMap(vector<shared_ptr<Actor>>& actors, const vector<shared_ptr<LightActor>>& lights)
{
	if (actors.empty())
		return;

	vector<shared_ptr<LightActor>> directSpotLights;
	shared_ptr<LightActor> pointLight = nullptr;
	vector<Matrix> VPs;
	array<Matrix, 6> PointVPs;
	bool bShadowUsePointLightSelected = false;

	for (int32 i = 0; i < lights.size(); ++i)
	{
		shared_ptr<LightActor> light = lights[i];
		ELightType lightType = light->GetLightType();

		if (lightType == ELightType::Directional || lightType == ELightType::Spot)
		{
			if (directSpotLights.size() < MAX_SHADOW_MAP_COUNT)
			{
				directSpotLights.push_back(light);
				VPs.push_back(light->GetLightVP());
				light->SetShadowMapIndex(VPs.size() - 1);
				light->SetShadowSRVInfo(_shadowTextures[i]->GetSRVBindingInfo());
			}
		}
		else
		{
			if (light->IsThisPointLightUseShadowMap())
			{
				assert(!bShadowUsePointLightSelected);
				pointLight = light;
				PointVPs = light->GetLightVPForPointLight();
				bShadowUsePointLightSelected = true;
			}
		}
	}

	assert(directSpotLights.size() == VPs.size() && VPs.size() <= MAX_SHADOW_MAP_COUNT);
	
	// Directional Light, SpotLight
	SHADER_PARAM_MANAGER->PushLightVPs(VPs);

	// Set Shadow PSO
	RENDER_MANAGER->SetPipelineState(_shadowPSO);
	CONTEXT->PSSetSamplers(2, 0, _shadowPSO->_pipelineState->_samplerState.GetAddressOf()); // Comparision Sampler
	for (int32 i = 0; i < VPs.size(); ++i)
	{
		DrawShadowMap(actors, i);
	}

	// Point Light
	if (bShadowUsePointLightSelected)
	{
		SHADER_PARAM_MANAGER->PushPointLightShadowDesc(PointVPs, pointLight->GetTransform()->GetWorldPosition());
		SHADER_PARAM_MANAGER->PushShadowCubeMapSRV(_shadowCubeTexture->GetSRVBindingInfo());

		// Set PointLight Shadow PSO
		GET_SINGLE(RenderManager)->SetPipelineState(_shadowPointLightPSO);
		DrawShadowCubeMap(pointLight, actors);
	}
}

void ShadowMap::DrawShadowMap(const vector<shared_ptr<Actor>>& actors, int32 index)
{
	SHADER_PARAM_MANAGER->PushShadowMapSRV (_shadowTextures[index]->GetSRVBindingInfo());
	SHADER_PARAM_MANAGER->PushCurrentLightVPIndex(index);

	ComPtr<ID3D11DepthStencilView> dsv = _shadowTextures[index]->GetDSV();
	CONTEXT->OMSetRenderTargets(0, nullptr, dsv.Get());
	CONTEXT->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	CONTEXT->RSSetViewports(1, &_shadowViewport);

	// 모든 물체의 depth 기록
	for (shared_ptr<Actor> actor : actors)
	{
		if (actor->IsCastShadowedActor())
		{
			actor->Render();
		}
	}
}

void ShadowMap::DrawShadowCubeMap(shared_ptr<LightActor> light, const vector<shared_ptr<Actor>>& actors)
{
	ComPtr<ID3D11RenderTargetView> RTV = _shadowCubeTexture->GetRTV();

	CONTEXT->OMSetRenderTargets(1, RTV.GetAddressOf(), nullptr);
	CONTEXT->ClearRenderTargetView(RTV.Get(), _cubeMapClearColor);
	CONTEXT->RSSetViewports(1, &_shadowViewport);
	
	for (shared_ptr<Actor> actor : actors)
	{
		if (actor->IsCastShadowedActor())
			actor->RenderShadowMap(true);
	}

	light->SetShadowSRVInfo(_shadowCubeTexture->GetSRVBindingInfo());
}

void ShadowMap::CreateShadowTexture()
{
	for (int32 i = 0; i < MAX_SHADOW_MAP_COUNT; ++i)
	{
		DepthMapTexture* t = new DepthMapTexture();
		t->CreateTexture(EShadowTextureType::Shadow);
		_shadowTextures[i] = t;
	}

	_shadowCubeTexture = new CubeDepthMapTexture();
	_shadowCubeTexture->CreateTexture(EShadowTextureType::ShadowCube);

	_depthMapTexture = new DepthMapTexture();
	_depthMapTexture->CreateTexture(EShadowTextureType::DepthMap);
}

void ShadowMap::SetShadowViewport()
{
	_shadowViewport.TopLeftX = 0.f;
	_shadowViewport.TopLeftY = 0.f;
	_shadowViewport.Width = static_cast<float>(SHADOW_MAP_SIZE);
	_shadowViewport.Height = static_cast<float>(SHADOW_MAP_SIZE);
	_shadowViewport.MinDepth = 0.0f;
	_shadowViewport.MaxDepth = 1.0f;
}
