#include "pch.h"
#include "CommonRenderResource.h"
#include "../Shader/VertexShader.h"
#include "../Shader/GeometryShader.h"
#include "../Shader/PixelShader.h"
#include "../Shader/ComputeShader.h"
#include "../Shader/ShaderInfo.h"
#include "../Buffer/InputLayout.h"
#include "../Buffer/VertexData.h"
#include "../PipelineState/PipelineState.h"

void CommonRenderResource::Initialize()
{
	InitProperties();

	// BasicMesh PSO
	{
		_basicMeshPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");

		_basicMeshPSO->_VS = make_shared<VertexShader>();
		_basicMeshPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
		
		_basicMeshPSO->_PS = make_shared<PixelShader>();
		_basicMeshPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);
		
		_basicMeshPSO->_inputLayout = make_shared<InputLayout>();
		_basicMeshPSO->_inputLayout->Create(VertexUVNormalTangentData::descs, _basicMeshPSO->_VS->GetBlob());
		
		_basicMeshPSO->_pipelineState = PipelineState::GetDefaultStates();
		_basicMeshPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	// StaticMesh PSO
	{
		_staticMeshPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicShader.hlsl");

		_staticMeshPSO->_VS = make_shared<VertexShader>();
		_staticMeshPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);

		_staticMeshPSO->_PS = make_shared<PixelShader>();
		_staticMeshPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);

		_staticMeshPSO->_inputLayout = make_shared<InputLayout>();
		_staticMeshPSO->_inputLayout->Create(VertexUVNormalTangentData::descs, _staticMeshPSO->_VS->GetBlob());

		_staticMeshPSO->_pipelineState = PipelineState::GetDefaultStates();
		_staticMeshPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	// CubeMap PSO
	{
		_cubeMapPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"CubeMapShader.hlsl");

		_cubeMapPSO->_VS = make_shared<VertexShader>();
		_cubeMapPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);

		_cubeMapPSO->_PS = make_shared<PixelShader>();
		_cubeMapPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);

		_cubeMapPSO->_inputLayout = make_shared<InputLayout>();
		_cubeMapPSO->_inputLayout->Create(VertexData::descs, _cubeMapPSO->_VS->GetBlob());

		_cubeMapPSO->_pipelineState = PipelineState::GetDefaultStates();
		_cubeMapPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}


	// Normal PSO
	{
		_normalPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"DrawNormal.hlsl");
		shaderInfo->AddGSShaderInfo();

		_normalPSO->_VS = make_shared<VertexShader>();
		_normalPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
		_normalPSO->_GS = make_shared<GeometryShader>();
		_normalPSO->_GS->Create(shaderInfo->_vsShaderPath, shaderInfo->_gsEntryName, shaderInfo->_gsVersion);
		_normalPSO->_PS = make_shared<PixelShader>();
		_normalPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);

		_normalPSO->_inputLayout = make_shared<InputLayout>();
		_normalPSO->_inputLayout->Create(VertexUVNormalTangentData::descs, _normalPSO->_VS->GetBlob());

		_normalPSO->_pipelineState = PipelineState::GetDefaultStates();
		_normalPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	}

	// Get DepthMap PSO
	{
		_getDepthMapPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"GetDepthMap.hlsl");

		_getDepthMapPSO->_VS = make_shared<VertexShader>();
		_getDepthMapPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
		
		_getDepthMapPSO->_inputLayout = make_shared<InputLayout>();
		_getDepthMapPSO->_inputLayout->Create(VertexData::descs, _getDepthMapPSO->_VS->GetBlob());
	
		_getDepthMapPSO->_pipelineState = PipelineState::GetShadowStates();
		_getDepthMapPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	// Show DepthMap PSO
	{
		_showDepthMapPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"ShowDepthMap.hlsl");

		_showDepthMapPSO->_VS = make_shared<VertexShader>();
		_showDepthMapPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
		_showDepthMapPSO->_PS = make_shared<PixelShader>();
		_showDepthMapPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);

		//_showDepthMapPSO->_inputLayout = make_shared<InputLayout>();
		//_showDepthMapPSO->_inputLayout->Create(VertexData::descs, _showDepthMapPSO->_VS->GetBlob());
	
		_showDepthMapPSO->_pipelineState = PipelineState::GetShadowStates();
		_showDepthMapPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	}

	// Shadow PSO
	{
		_shadowPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"GetShadowMap.hlsl");

		_shadowPSO->_VS = make_shared<VertexShader>();
		_shadowPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
		
		_shadowPSO->_inputLayout = make_shared<InputLayout>();
		_shadowPSO->_inputLayout->Create(VertexUVNormalTangentData::descs, _shadowPSO->_VS->GetBlob());
		
		_shadowPSO->_pipelineState = PipelineState::GetShadowStates();
		_shadowPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	// Shadow PointLight PSO
	{
		_shadowPointLightPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"GetShadowMapForPointLight.hlsl");
		shaderInfo->AddGSShaderInfo();

		_shadowPointLightPSO->_VS = make_shared<VertexShader>();
		_shadowPointLightPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
		_shadowPointLightPSO->_GS = make_shared<GeometryShader>();
		_shadowPointLightPSO->_GS->Create(shaderInfo->_vsShaderPath, shaderInfo->_gsEntryName, shaderInfo->_gsVersion);
		_shadowPointLightPSO->_PS = make_shared<PixelShader>();
		_shadowPointLightPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);
		
		_shadowPointLightPSO->_inputLayout = make_shared<InputLayout>();
		_shadowPointLightPSO->_inputLayout->Create(VertexData::descs, _shadowPointLightPSO->_VS->GetBlob());
		
		_shadowPointLightPSO->_pipelineState = PipelineState::GetShadowStates();
		_shadowPointLightPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	// Draw Stencil PSO
	{
		_drawStencilPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"WriteStencil.hlsl");

		_drawStencilPSO->_VS = make_shared<VertexShader>();
		_drawStencilPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);

		_drawStencilPSO->_PS = make_shared<PixelShader>();
		_drawStencilPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);
		
		_drawStencilPSO->_inputLayout = make_shared<InputLayout>();
		_drawStencilPSO->_inputLayout->Create(VertexUVNormalTangentData::descs, _drawStencilPSO->_VS->GetBlob());
		
		_drawStencilPSO->_pipelineState = PipelineState::GetDrawStencilStates();
		_drawStencilPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		_drawStencilPSO->_stencilRef = 1;
	}

	// Using Stencil PSO
	{
		_usingStencilPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"BasicMeshShader.hlsl");

		_usingStencilPSO->_VS = make_shared<VertexShader>();
		_usingStencilPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
		
		_usingStencilPSO->_PS = make_shared<PixelShader>();
		_usingStencilPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);
		
		_usingStencilPSO->_inputLayout = make_shared<InputLayout>();
		_usingStencilPSO->_inputLayout->Create(VertexUVNormalTangentData::descs, _drawStencilPSO->_VS->GetBlob());
		
		_usingStencilPSO->_pipelineState = PipelineState::GetUsingStencilStates();
		_usingStencilPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		
		_usingStencilPSO->_stencilRef = 1;
	}

	// post Effect PSO
	{
		_postEffectPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"PostEffect.hlsl");

		_postEffectPSO->_VS = make_shared<VertexShader>();
		_postEffectPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
		_postEffectPSO->_PS = make_shared<PixelShader>();
		_postEffectPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);

		_postEffectPSO->_inputLayout = make_shared<InputLayout>();
		_postEffectPSO->_inputLayout->Create(VertexData::descs, _postEffectPSO->_VS->GetBlob());
		
		_postEffectPSO->_pipelineState = PipelineState::GetDefaultStates();
		_postEffectPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	// shaderToy PSO
	{
		_shaderToyPSO = make_shared<PSO>();
		shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"GetQuadVS.hlsl", L"SeaScape.hlsl");
		//shared_ptr<ShaderInfo> shaderInfo = make_shared<ShaderInfo>(L"GetQuadVS.hlsl", L"ShaderToy1.hlsl");

		_shaderToyPSO->_VS = make_shared<VertexShader>();
		_shaderToyPSO->_VS->Create(shaderInfo->_vsShaderPath, shaderInfo->_vsEntryName, shaderInfo->_vsVersion);
		_shaderToyPSO->_PS = make_shared<PixelShader>();
		_shaderToyPSO->_PS->Create(shaderInfo->_psShaderPath, shaderInfo->_psEntryName, shaderInfo->_psVersion);
		
		_shaderToyPSO->_inputLayout = make_shared<InputLayout>();
		_shaderToyPSO->_inputLayout->Create(VertexData::descs, _shaderToyPSO->_VS->GetBlob());
		
		_shaderToyPSO->_pipelineState = PipelineState::GetDefaultStates();
		_shaderToyPSO->_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

	// Test Compute PSO
	{
		_testComputePSO = make_shared<PSO>();
		_testComputePSO->_CS = make_shared<ComputeShader>();
		_testComputePSO->_CS->Create(L"CSPrac1.hlsl", "main", "cs_5_0");
	}
}

void CommonRenderResource::InitProperties()
{
	
}
