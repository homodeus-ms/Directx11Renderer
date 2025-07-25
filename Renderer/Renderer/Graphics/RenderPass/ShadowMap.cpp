#include "pch.h"
#include "ShadowMap.h"
#include "Actor/Actor.h"
#include "Actor/LightActor.h"
#include "Managers/ShaderParameterManager.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Buffer/VertexData.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Resource/Texture.h"

ShadowMap::ShadowMap()
{
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::Construct()
{
	CreateShadowTexture();
	CreateShadowDSV();
	CreateShadowSRV();
	CreateShadowCubeRTV();
	SetShadowViewport();
	CreateShadowMapResources();
}

void ShadowMap::CreateShadowMap(vector<shared_ptr<Actor>>& actors, const vector<shared_ptr<LightActor>>& lights)
{
	// Shadow Map을 만드는 것이므로 조명 하나당 모든 물체를 돌면서 Depth값 기록
	for (int32 i = 0; i < lights.size(); ++i)
	{
		shared_ptr<LightActor> light = lights[i];
		ELightType lightType = light->GetLightType();

		if (lightType == ELightType::Directional || lightType == ELightType::Spot)
			DrawShadowMap(light, actors, i);
		else
			DrawShadowCubeMap(light, actors);
	}
}

void ShadowMap::DrawShadowMap(shared_ptr<LightActor> light, const vector<shared_ptr<Actor>>& actors, int32 index)
{
	CONTEXT->OMSetRenderTargets(0, nullptr, _shadowDSVs[index].Get());
	CONTEXT->ClearDepthStencilView(_shadowDSVs[index].Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	CONTEXT->RSSetViewports(1, &_shadowViewport);
	
	SHADER_PARAM_MANAGER->AddShadowData(light->GetLightVP());
	SHADER_PARAM_MANAGER->UpdateShadowMapData();

	// 모든 물체의 depth 기록
	for (shared_ptr<Actor> actor : actors)
	{
		if (actor->IsCastShadowedActor())
			actor->RenderDepthOnly(false);
	}

	SHADER_PARAM_MANAGER->CleanUpShadowMapBuffers();  // 조명 리셋

	// 해당 라이트에 SRVBindingInfo를 넣어줌, 나중에 각 Light마다 자신이 사용하게 될 SRV를 꺼내줄 수 있게
	light->SetShadowSRVInfo(_shadowSRVBindingInfos[index]);
}

void ShadowMap::DrawShadowCubeMap(shared_ptr<LightActor> light, const vector<shared_ptr<Actor>>& actors)
{
	//const vector<Matrix>& VPs = light->GetLightVPForPointLight();
	vector<Matrix> views;
	vector<Matrix> VPs;
	light->GetLightVPForPointLight(views, VPs);

	float clearColor[4] = { 0.5f, 0.f, 0.f, 1.f };

	// Point Light
	for (int32 i = 0; i < 6; ++i)
	{
		CONTEXT->OMSetRenderTargets(1, _shadowCubeRTVs[i].GetAddressOf(), nullptr);
		CONTEXT->ClearRenderTargetView(_shadowCubeRTVs[i].Get(), clearColor);
		CONTEXT->RSSetViewports(1, &_shadowViewport);
	
		SHADER_PARAM_MANAGER->AddShadowData(views[i], VPs[i]);
		SHADER_PARAM_MANAGER->UpdateShadowMapData();
	
		// 모든 물체의 depth 기록
		for (shared_ptr<Actor> actor : actors)
		{
			if (actor->IsCastShadowedActor())
				actor->RenderDepthOnly(true);
		}
		SHADER_PARAM_MANAGER->CleanUpShadowMapBuffers();
	}
	
	light->SetShadowSRVInfo(_shadowCubeSRVBindingInfo);
}

void ShadowMap::CreateShadowTexture()
{
	for (int32 i = 0; i < MAX_SHADOW_MAP_COUNT; ++i)
	{
		D3D11_TEXTURE2D_DESC desc;
		::ZeroMemory(&desc, sizeof(desc));

		desc.Width = SHADOW_MAP_SIZE;
		desc.Height = SHADOW_MAP_SIZE;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_TYPELESS; // 깊이와 SRV 둘 다 사용하려면 TYPELESS
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _shadowTextures[i].GetAddressOf());
		check(hr);
	}

	// For Cube Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		::ZeroMemory(&desc, sizeof(desc));

		desc.Width = SHADOW_MAP_SIZE;
		desc.Height = SHADOW_MAP_SIZE;
		desc.MipLevels = 1;
		desc.ArraySize = 6;
		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		HRESULT hr = DEVICE->CreateTexture2D(&desc, nullptr, _shadowCubeTexture.GetAddressOf());
		check(hr);
	}
}

void ShadowMap::CreateShadowDSV()
{
	for (int32 i = 0; i < MAX_SHADOW_MAP_COUNT; ++i)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Format = DXGI_FORMAT_D32_FLOAT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		HRESULT hr = DEVICE->CreateDepthStencilView(_shadowTextures[i].Get(), &desc, _shadowDSVs[i].GetAddressOf());
		check(hr);
	}
}

void ShadowMap::CreateShadowSRV()
{
	for (int32 i = 0; i < MAX_SHADOW_MAP_COUNT; ++i)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		::ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32_FLOAT; // Shader에서 사용할 format
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = 1;

		HRESULT hr = DEVICE->CreateShaderResourceView(_shadowTextures[i].Get(), &desc, _shadowSRVs[i].GetAddressOf());
		check(hr);

		// SRV Binding Info
		_shadowSRVBindingInfos[i] = make_shared<SRVBindingInfo>();
		_shadowSRVBindingInfos[i]->slot = SHADOW_SLOT_NUM;
		_shadowSRVBindingInfos[i]->stage = EShaderStage::PsStage;
		_shadowSRVBindingInfos[i]->srv = _shadowSRVs[i];
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MipLevels = 1;
		srvDesc.TextureCube.MostDetailedMip = 0;
		
		HRESULT hr = DEVICE->CreateShaderResourceView(_shadowCubeTexture.Get(), &srvDesc, _shadowCubeSRV.GetAddressOf());
		check(hr);

		// SRV Binding Info
		_shadowCubeSRVBindingInfo = make_shared<SRVBindingInfo>();
		_shadowCubeSRVBindingInfo->slot = SHADOW_SLOT_NUM + MAX_SHADOW_MAP_COUNT;
		_shadowCubeSRVBindingInfo->stage = EShaderStage::PsStage;
		_shadowCubeSRVBindingInfo->srv = _shadowCubeSRV;
	}
}

void ShadowMap::CreateShadowCubeRTV()
{
	// 렌더 타겟 뷰 6개 (큐브맵의 각 면)
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.MipSlice = 0;
	rtvDesc.Texture2DArray.ArraySize = 1;

	for (int i = 0; i < 6; ++i)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = i;

		HRESULT hr = DEVICE->CreateRenderTargetView(
			_shadowCubeTexture.Get(),
			&rtvDesc,
			_shadowCubeRTVs[i].GetAddressOf()
		);
		check(hr);
	}
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
	_defualtShaderInfo = make_shared<ShaderInfo>(SHADER_NAME);
	_pointLightShaderInfo = make_shared<ShaderInfo>(POINT_LIGHT_SHADER_NAME);

	_resources.defaultVertexShader = make_shared<VertexShader>();
	_resources.defaultVertexShader->Create(_defualtShaderInfo->_shaderPath, _defualtShaderInfo->_vsEntryName, _defualtShaderInfo->_vsVersion);

	_resources.pointLightVertexShader = make_shared<VertexShader>();
	_resources.pointLightVertexShader->Create(_pointLightShaderInfo->_shaderPath, _pointLightShaderInfo->_vsEntryName, _pointLightShaderInfo->_vsVersion);

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



