#include "pch.h"
#include "RenderManager.h"
#include "Actor/Actor.h"
#include "Actor/LightActor.h"
#include "Graphics/RenderPass/ShadowMap.h"
#include "Managers/SceneManager.h"
#include "Managers/ShaderParameterManager.h"
#include "Resource/Material/MaterialBase.h"
#include "Resource/Material/Material.h"
#include "Resource/Material/IBLMaterial.h"
#include "Components/CameraComponent.h"
#include "Components/Transform.h"
#include "Components/ReflectComponent.h"
#include "Graphics/Shader/ShaderInfo.h"
#include "Graphics/PipelineState/PipelineState.h"
#include "Graphics/Filter/FilterManager.h"
#include "Graphics/RenderPass/CommonRenderResource.h"
#include "Graphics/RenderPass/PSO.h"
#include "Graphics/RenderPass/PostEffect.h"
#include "Graphics/Buffer/InputLayout.h"
#include "Graphics/Shader/VertexShader.h"
#include "Graphics/Shader/GeometryShader.h"
#include "Graphics/Shader/PixelShader.h"
#include "Graphics/Shader/ComputeShader.h"

void RenderManager::BeginPlay()
{
	GET_SINGLE(FilterManager)->Construct();
	_postEffect = make_shared<PostEffect>();
	_postEffect->Construct();

	//GET_SINGLE(CommonRenderResource)->Initialize();
	_normalPSO = GET_SINGLE(CommonRenderResource)->_normalPSO;
	_basicMeshPSO = GET_SINGLE(CommonRenderResource)->_basicMeshPSO;
	_staticMeshPSO = GET_SINGLE(CommonRenderResource)->_staticMeshPSO;
	_cubeMapPSO = GET_SINGLE(CommonRenderResource)->_cubeMapPSO;
	_drawStencilPSO = GET_SINGLE(CommonRenderResource)->_drawStencilPSO;
	_usingStencilPSO = GET_SINGLE(CommonRenderResource)->_usingStencilPSO;
	_drawDebugQuadPSO = GET_SINGLE(CommonRenderResource)->_showDepthMapPSO;
	_shaderToyPSO = GET_SINGLE(CommonRenderResource)->_shaderToyPSO;
}

void RenderManager::Render(vector<shared_ptr<Actor>>& actors)
{
	RenderCS();
	return;


	vector<shared_ptr<LightActor>> lightActors;
	vector<shared_ptr<Actor>> shadowedActors;  // 그림자가 반영될 actors
	shared_ptr<Actor> shadowMapDebugActor;  // 디버그용 actor
	vector<shared_ptr<Actor>> drawTargets;  // 실제 렌더할 모든 actors
	vector<shared_ptr<Actor>> reflectActors;

	for (shared_ptr<Actor> actor : actors)
	{
		if (actor->GetActorType() == EActorType::DebugActor)
		{
			shadowMapDebugActor = actor;
			continue;
		}

		if (actor->GetActorType() == EActorType::LightActor)
		{
			lightActors.push_back(static_pointer_cast<LightActor>(actor));
		}
		else if (actor->IsCastShadowedActor())
		{
			shadowedActors.push_back(actor);
		}
		
		if (actor->GetActorType() == EActorType::ReflectActor)
		{
			reflectActors.push_back(actor);
		}
		else
		{
			drawTargets.push_back(actor);
		}
	}

	// ShaderToy Test
	{
		GRAPHICS->RenderMSAABegin();  // 실제 물체를 그리는 RenderTarget, viewport등 셋팅
		UpdateCommonDatasPerFrame(lightActors);  // Global데이터, SamplerState, RSState등
		RenderShaderToyDemos();
		
		FILTER_MANAGER->RenderFilters();
		SHADER_PARAM_MANAGER->CleanUpDatasAfterRender();
		return;
	}


	
	// TODO : HeightMap 사용때문에 기존의 간단한 GetDepthMapShader가 필요가 없어졌음..
	SetPipelineState(_basicMeshPSO);
	CONTEXT->PSSetShader(nullptr, nullptr, 0);
	_depthMapSRVInfo = GetDepthMapSRV(actors);

	// Render ShadowMap
	_shadowMap->CreateAndDrawShadowMap(shadowedActors, lightActors); // 섀도우 맵 생성
	
	GRAPHICS->RenderMSAABegin();  // 실제 물체를 그리는 RenderTarget, viewport등 셋팅
	UpdateCommonDatasPerFrame(lightActors);  // Global데이터, SamplerState, RSState등
	
	// Draw ShadowMap Debug
	//if (SCENE->ShouldDrawDebugShadowMap())
	//{
	//	shadowMapDebugActor->Render();    // 섀도우 맵 디버그용
	//}
	
	DrawActors(drawTargets);
	//DrawNormals(drawTargets);
	DrawMirrorScene(drawTargets, reflectActors);

	// Post Effect - Fog
	SetPipelineState(GET_SINGLE(CommonRenderResource)->_postEffectPSO);
	PostEffectParams params;
	params.srvs.push_back(GRAPHICS->GetResolvedSRV().Get());
	params.srvs.push_back(_depthMapSRVInfo->srv.Get());
	_postEffect->Render(params);

	ComPtr<ID3D11ShaderResourceView> postEffected = _postEffect->GetSRV();
	FILTER_MANAGER->RenderFilters();
	
	SHADER_PARAM_MANAGER->CleanUpDatasAfterRender();
}

void RenderManager::SetCubeMapActor(shared_ptr<Actor> cubeMapActor)
{
	_cubeMapActor = cubeMapActor;
}

void RenderManager::UpdateCommonDatasPerFrame(const vector<shared_ptr<LightActor>>& lightActors)
{
	// 공통으로 사용하는 cbuffer들 셋팅 : cbuffer에 올라간 값은 값 유지가 됨
	// 따라서 모든 액터가 공통으로 사용할 값들은 한 프레임에 한 번만 GPU로 업로드하면 됨

	SHADER_PARAM_MANAGER->PushGlobalData(SCENE->GetCurrCameraV(), SCENE->GetCurrCameraP());

	// Light, EnvLight source, ShadowMap SRV등
	SHADER_PARAM_MANAGER->BindCommonResources();
}

void RenderManager::DrawActors(const vector<shared_ptr<Actor>>& actors)
{
	CONTEXT->OMSetBlendState(NULL, NULL, 0xffffffff);
	SetPipelineState(_basicMeshPSO);
	// Render Objects
	for (const shared_ptr<Actor>& actor : actors)
	{
		if (actor->GetMeshType() == EResourceType::BasicMesh)
		{
			ChangeShaders(_basicMeshPSO);
		}
		else if (actor->GetMeshType() == EResourceType::StaticMesh)
		{
			ChangeShaders(_staticMeshPSO);
		}
		actor->Render();
	}

	// CubeMap
	SetPipelineState(_cubeMapPSO);
	if (_cubeMapActor)
		_cubeMapActor->Render();
}

void RenderManager::DrawStencil(const vector<shared_ptr<Actor>>& stenciledActors, shared_ptr<PSO> pso)
{
	SetPipelineState(pso);
	for (const shared_ptr<Actor>& actor : stenciledActors)
		actor->Render();
}

void RenderManager::DrawNormals(const vector<shared_ptr<Actor>>& actors)
{
	SetPipelineState(_normalPSO);
	for (const shared_ptr<Actor>& actor : actors)
	{
		if (actor->ShouldDrawNormal())
		{
			actor->RenderDrawNormal();
		}
	}
}

void RenderManager::DrawMirrorScene(vector<shared_ptr<Actor>>& actors, const vector<shared_ptr<Actor>>& reflectActors)
{
	vector<Matrix> reflectMatrices;
	for (uint32 i = 0; i < reflectActors.size(); ++i)
	{
		shared_ptr<Actor> mirror = reflectActors[i];
		shared_ptr<ReflectComponent> component = static_pointer_cast<ReflectComponent>(mirror->GetReflectComponentOrNull());
		assert(component != nullptr);

		reflectMatrices.push_back(component->GetReflectMatrix());
	}
	
	assert(reflectMatrices.size() == reflectActors.size());
	SetPipelineState(_drawStencilPSO);
	vector<uint32> stencilValue;

	for (uint32 i = 0; i < reflectActors.size(); ++i)
	{
		stencilValue.push_back(i + 1);
		CONTEXT->OMSetDepthStencilState(_drawStencilPSO->_pipelineState->_dsState.Get(), i + 1);
		reflectActors[i]->Render();
	}

	Matrix V = SCENE->GetCurrCameraV();
	Matrix P = SCENE->GetCurrCameraP();

	CONTEXT->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	for (uint32 i = 0; i < reflectActors.size(); ++i)
	{
		GRAPHICS->ClearDepth();

		Matrix reflectMatrix = reflectMatrices[i];
		SHADER_PARAM_MANAGER->PushGlobalData(reflectMatrix * V, P);

		CONTEXT->OMSetDepthStencilState(_usingStencilPSO->_pipelineState->_dsState.Get(), stencilValue[i]);
		CONTEXT->RSSetState(_usingStencilPSO->_pipelineState->_rsState.Get());

		// Draw Reflected Actors
		for (const shared_ptr<Actor>& actor : actors)
		{
			if (actor->GetMeshType() == EResourceType::BasicMesh)
			{
				ChangeShaders(_basicMeshPSO);
			}
			else if (actor->GetMeshType() == EResourceType::StaticMesh)
			{
				ChangeShaders(_staticMeshPSO);
			}
			actor->Render();
		}

		// Draw Reflected CubeMap
		SetPipelineState(_cubeMapPSO);
		CONTEXT->OMSetDepthStencilState(_usingStencilPSO->_pipelineState->_dsState.Get(), stencilValue[i]);
		CONTEXT->RSSetState(_usingStencilPSO->_pipelineState->_rsState.Get());
		if (_cubeMapActor)
			_cubeMapActor->Render();

		// Draw Mirror with Stencil + Blend
		SHADER_PARAM_MANAGER->PushGlobalData(V, P); 
		SetPipelineState(_basicMeshPSO);
		float blendFactor = reflectActors[i]->GetMaterials()[0]->GetBlendFactor();
		const float blendColor[] = { blendFactor, blendFactor, blendFactor, 1.f };
		CONTEXT->OMSetDepthStencilState(_usingStencilPSO->_pipelineState->_dsState.Get(), stencilValue[i]);
		CONTEXT->OMSetBlendState(_usingStencilPSO->_pipelineState->_blendState.Get(), blendColor, 0xffffffff);
		reflectActors[i]->Render();

		CONTEXT->OMSetBlendState(NULL, NULL, 0xffffffff);
	}
}

shared_ptr<SRVBindingInfo> RenderManager::GetDepthMapSRV(const vector<shared_ptr<Actor>>& actors)
{
	return _shadowMap->DrawDepthMap(actors);
}

void RenderManager::DrawDepthMapAndShowForDebug(const vector<shared_ptr<Actor>>& actors)
{
	GRAPHICS->RenderMSAABegin();
	CONTEXT->PSSetShaderResources(_depthMapSRVInfo->slot, 1, _depthMapSRVInfo->srv.GetAddressOf());
	//SHADER_PARAM_MANAGER->PushGlobalData(SCENE->GetCurrCameraV(), SCENE->GetCurrCameraP());
	SetPipelineState(_basicMeshPSO);
	ChangeShaders(_drawDebugQuadPSO);

	CONTEXT->Draw(6, 0);

	FILTER_MANAGER->RenderFilters();
	SHADER_PARAM_MANAGER->CleanUpDatasAfterRender();
}

void RenderManager::RenderShaderToyDemos()
{
	SetPipelineState(_shaderToyPSO);

	static float sumTime = 0.f;
	static int32 frame = 0;

	float dt = TIME_MANAGER->GetDeltaTime();

	ShaderToyData data;
	data.iResolution = Vec3(GWinSizeX, GWinSizeY, 1.f);
	data.iTime = sumTime += dt;
	data.iFrame = frame++;
	data.iTimeDelta = dt;
	data.iFrameRate = static_cast<float>(TIME_MANAGER->GetFps());

	SHADER_PARAM_MANAGER->PushShaderToyDataImmediately(data);
	
	CONTEXT->Draw(6, 0);
}

void RenderManager::SetPipelineState(shared_ptr<class PSO> pso)
{
	CONTEXT->IASetInputLayout(pso->_inputLayout->GetComPtr().Get());
	CONTEXT->IASetPrimitiveTopology(pso->_topology);

	ChangeShaders(pso);

	if (!_bWireFrameMode)
		CONTEXT->RSSetState(pso->_pipelineState->_rsState.Get());
	else
		CONTEXT->RSSetState(pso->_pipelineState->_wireframeState.Get());

	CONTEXT->PSSetSamplers(0, 1, pso->_pipelineState->_samplerState.GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, pso->_pipelineState->_clampSamplerState.GetAddressOf());
	CONTEXT->OMSetDepthStencilState(pso->_pipelineState->_dsState.Get(), pso->_stencilRef);
	CONTEXT->OMSetBlendState(pso->_pipelineState->_blendState.Get(), &pso->_pipelineState->_blendFactor, pso->_pipelineState->_sampleMask);
}




void RenderManager::ChangeShaders(shared_ptr<PSO> pso)
{
	if (pso->_VS)
		CONTEXT->VSSetShader(pso->_VS->GetComPtr().Get(), nullptr, 0);

	if (pso->_GS)
		CONTEXT->GSSetShader(pso->_GS->GetComPtr().Get(), nullptr, 0);
	else
		ClearGSShader();

	if (pso->_PS)
		CONTEXT->PSSetShader(pso->_PS->GetComPtr().Get(), nullptr, 0);
}

void RenderManager::SetComputeShader(shared_ptr<class PSO> pso)
{
	CONTEXT->VSSetShader(nullptr, 0, 0);
	CONTEXT->PSSetShader(nullptr, 0, 0);
	CONTEXT->GSSetShader(nullptr, 0, 0);
	CONTEXT->HSSetShader(nullptr, 0, 0);
	CONTEXT->DSSetShader(nullptr, 0, 0);
	CONTEXT->CSSetShader(pso->_CS->GetComPtr().Get(), 0, 0);
}

void RenderManager::ClearGSShader()
{
	CONTEXT->GSSetShader(nullptr, 0, 0);
}

void RenderManager::RenderCS()
{
	if (_testComputePSO == nullptr)
		_testComputePSO = GET_SINGLE(CommonRenderResource)->_testComputePSO;

	SetComputeShader(_testComputePSO);

	CONTEXT->CSSetUnorderedAccessViews(0, 1, GRAPHICS->GetUAV().GetAddressOf(), NULL);
	CONTEXT->Dispatch(UINT(ceil(VIEW_X / 32.f)), UINT(ceil(VIEW_Y / 32.f)), 1);

	ComputeShaderBarrier();

	GRAPHICS->RenderEnd();
}

void RenderManager::ComputeShaderBarrier()
{
	ID3D11ShaderResourceView* emptySrvs[6] = { 0, };
	ID3D11UnorderedAccessView* emptyUavs[6] = { 0, };
	CONTEXT->CSSetShaderResources(0, 6, emptySrvs);
	CONTEXT->CSSetUnorderedAccessViews(0, 6, emptyUavs, NULL);
}
