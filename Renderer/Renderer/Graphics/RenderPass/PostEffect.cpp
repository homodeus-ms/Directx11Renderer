#include "pch.h"
#include "PostEffect.h"
#include "CommonRenderResource.h"
#include "../PipelineState/PipelineState.h"
#include "PSO.h"
#include "Resource/Texture/RenderTexture.h"
#include "Managers/ShaderParameterManager.h"

void PostEffect::Construct()
{
	_postEffectPSO = GET_SINGLE(CommonRenderResource)->_postEffectPSO;
	CreateViewport();
	_texture = make_shared<RenderTexture>();
	_texture->CreateTexture(_viewport.Width, _viewport.Height, POST_EFFECT_SRV_START);
}

void PostEffect::Render(const PostEffectParams& params)
{
	const vector<ID3D11ShaderResourceView*>& v = params.srvs;
	CONTEXT->PSSetShaderResources(params.startSlot, v.size(), v.data());

	//CONTEXT->OMSetRenderTargets(1, _texture->GetRTV().GetAddressOf(), nullptr);
	//CONTEXT->ClearRenderTargetView(_texture->GetRTV().Get(), (float*)&RENDERER->GetGameDesc().clearColor);
	//CONTEXT->RSSetViewports(1, &_viewport);
	
	SHADER_PARAM_MANAGER->PushPostEffectDescImmediately(_postEffectData);

	CONTEXT->Draw(6, 0);
}

ComPtr<ID3D11ShaderResourceView> PostEffect::GetSRV()
{
	return _texture->GetSRV();
}

void PostEffect::CreateViewport()
{
	ZeroMemory(&_viewport, sizeof(D3D11_VIEWPORT));

	//_viewport.TopLeftX = GWinSizeX == VIEW_X ? 0 : GViewportStartX;
	_viewport.TopLeftX = 0;
	_viewport.TopLeftY = 0;
	_viewport.Width = static_cast<float>(GWinSizeX);
	_viewport.Height = static_cast<float>(GWinSizeY);
	//_viewport.Width = static_cast<float>(VIEW_X);
	//_viewport.Height = static_cast<float>(VIEW_Y);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}
