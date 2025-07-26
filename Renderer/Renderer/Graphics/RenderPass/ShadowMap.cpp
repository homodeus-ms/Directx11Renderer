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
#include "Resource/Texture/ShadowTexture.h"
#include "Resource/Texture/ShadowCubeTexture.h"
#include "Components/Transform.h"

ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
	SAFE_DELETE(_shadowTexture);

	SAFE_DELETE(_shadowCubeTexture);
}

void ShadowMap::Construct()
{
	CreateShadowTexture();
	SetShadowViewport();
	CreateShadowMapResources();
}

void ShadowMap::CreateShadowMap(vector<shared_ptr<Actor>>& actors, const vector<shared_ptr<LightActor>>& lights)
{
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
				light->SetShadowMapIndex(i);
				light->SetShadowSRVInfo(_shadowTexture->GetSRVBindingInfo());
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
	int32 instanceCount = VPs.size();
	if (instanceCount != 0)
	{
		SHADER_PARAM_MANAGER->PushShadowMapSRV(_shadowTexture->GetSRVBindingInfo());
		DrawShadowMap(actors, instanceCount);
	}

	// Point Light
	if (bShadowUsePointLightSelected)
	{
		SHADER_PARAM_MANAGER->PushPointLightShadowDesc(PointVPs, pointLight->GetTransform()->GetWorldPosition());
		SHADER_PARAM_MANAGER->PushShadowCubeMapSRV(_shadowCubeTexture->GetSRVBindingInfo());
		DrawShadowCubeMap(pointLight, actors);
	}
}

void ShadowMap::DrawShadowMap(const vector<shared_ptr<Actor>>& actors, int32 drawShadowMapCount)
{
	ComPtr<ID3D11DepthStencilView> dsv = _shadowTexture->GetDSV();

	CONTEXT->OMSetRenderTargets(0, nullptr, dsv.Get());
	CONTEXT->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	CONTEXT->RSSetViewports(1, &_shadowViewport);

	// 모든 물체의 depth 기록
	for (shared_ptr<Actor> actor : actors)
	{
		if (actor->IsCastShadowedActor())
			actor->RenderShadowMap(false, drawShadowMapCount);
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
	_shadowTexture = new ShadowTexture();
	_shadowTexture->CreateTexture();

	_shadowCubeTexture = new ShadowCubeTexture();
	_shadowCubeTexture->CreateTexture();
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

void ShadowMap::CreateShadowMapResources()
{
	_defaultShaderInfo = make_shared<ShaderInfo>(SHADER_NAME);
	_defaultShaderInfo->AddGSShaderInfo();
	_pointLightShaderInfo = make_shared<ShaderInfo>(POINT_LIGHT_SHADER_NAME);
	_pointLightShaderInfo->AddGSShaderInfo();

	_resources.defaultVertexShader = make_shared<VertexShader>();
	_resources.defaultVertexShader->Create(_defaultShaderInfo->_shaderPath, _defaultShaderInfo->_vsEntryName, _defaultShaderInfo->_vsVersion);

	_resources.defaultGeometryShader = make_shared<GeometryShader>();
	_resources.defaultGeometryShader->Create(_defaultShaderInfo->_shaderPath, _defaultShaderInfo->_gsEntryName, _defaultShaderInfo->_gsVersion);

	_resources.defaultPixelShader = make_shared<PixelShader>();
	_resources.defaultPixelShader->Create(_defaultShaderInfo->_shaderPath, _defaultShaderInfo->_psEntryName, _defaultShaderInfo->_psVersion);

	_resources.pointLightVertexShader = make_shared<VertexShader>();
	_resources.pointLightVertexShader->Create(_pointLightShaderInfo->_shaderPath, _pointLightShaderInfo->_vsEntryName, _pointLightShaderInfo->_vsVersion);

	_resources.pointLightGeometryShader = make_shared<GeometryShader>();
	_resources.pointLightGeometryShader->Create(_pointLightShaderInfo->_shaderPath, _pointLightShaderInfo->_gsEntryName, _pointLightShaderInfo->_gsVersion);

	_resources.pointLightPixelShader = make_shared<PixelShader>();
	_resources.pointLightPixelShader->Create(_pointLightShaderInfo->_shaderPath, _pointLightShaderInfo->_psEntryName, _pointLightShaderInfo->_psVersion);



	const vector<D3D11_INPUT_ELEMENT_DESC>& desc = VertexData::descs;
	_resources.inputLayout = make_shared<InputLayout>();
	ComPtr<ID3DBlob> blob = _resources.defaultVertexShader->GetBlob();
	_resources.inputLayout->Create(desc, blob);

	// Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 100;
		desc.SlopeScaledDepthBias = 3.f;
		desc.DepthBiasClamp = 0.0f;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = false;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = DEVICE->CreateRasterizerState(&desc, _resources.rasterizerState.GetAddressOf());
		check(hr);
	}

	// Depth Stencil State
	{ 
		D3D11_DEPTH_STENCIL_DESC desc = {};
		desc.DepthEnable = TRUE;							// 깊이 테스트 활성화
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;  // 깊이값 기록 허용
		desc.DepthFunc = D3D11_COMPARISON_LESS;			// 더 가까운 픽셀만 통과
		desc.StencilEnable = FALSE;						// 그림자맵에서는 Stencil 불필요

		ID3D11DepthStencilState* shadowDepthState = nullptr;
		HRESULT hr = DEVICE->CreateDepthStencilState(&desc, _resources.depthStencilState.GetAddressOf());
		check(hr);
	}
}



