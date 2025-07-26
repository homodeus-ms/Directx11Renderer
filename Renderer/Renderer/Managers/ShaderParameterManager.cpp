#include "pch.h"
#include "ShaderParameterManager.h"
#include "Components/CameraComponent.h"
#include "Resource/Material.h"
#include "Graphics/RenderPass/ShadowMapResources.h"


void ShaderParameterManager::BeginPlay()
{
	// Register Default Constant Buffers
	
	RegisterBuffer<GlobalDesc>("Global", static_cast<uint8>(EConstBufferRegisterNumber::Global), EShaderStage::Both);
	RegisterBuffer<TransformDesc>("Transform", static_cast<uint8>(EConstBufferRegisterNumber::Transform), EShaderStage::VsStage);
	RegisterBuffer<DirectionalLightDesc>("DirectionalLight", static_cast<uint8>(EConstBufferRegisterNumber::DirectionalLight), EShaderStage::PsStage);
	RegisterBuffer<SpotLightBuffer>("SpotLight", static_cast<uint8>(EConstBufferRegisterNumber::SpotLight), EShaderStage::PsStage);
	RegisterBuffer<PointLightBuffer>("PointLight", static_cast<uint8>(EConstBufferRegisterNumber::PointLight), EShaderStage::PsStage);
	
	RegisterBuffer<MaterialDesc>("Material", static_cast<uint8>(EConstBufferRegisterNumber::Material), EShaderStage::PsStage);
	RegisterBuffer<BoneBuffer>("BoneBuffer", static_cast<uint8>(EConstBufferRegisterNumber::BoneBuffer), EShaderStage::VsStage);
	RegisterBuffer<BoneIndex>("BoneIndex", static_cast<uint8>(EConstBufferRegisterNumber::BoneIndex), EShaderStage::VsStage);
	RegisterBuffer<ShadowDataDesc>("Shadow", static_cast<uint8>(EConstBufferRegisterNumber::ShadowData), EShaderStage::Both);
	RegisterBuffer<PointShadowDataDesc>("PointShadow", static_cast<uint8>(EConstBufferRegisterNumber::PointShadowData), EShaderStage::PsStage | EShaderStage::GsStage);
}

void ShaderParameterManager::Update()
{
}

void ShaderParameterManager::PushGlobalData(const Matrix& view, const Matrix& projection)
{
	GlobalDesc desc;
	desc.V = view;
	desc.P = projection;
	desc.VP = desc.V * desc.P;

	Matrix invV = view.Invert();
	desc.CameraPosition = { invV._41, invV._42, invV._43 };
	desc.bEnvLightUsing = _bEnvLigthOn ? 1 : 0;

	UpdateData("Global", desc);
}

void ShaderParameterManager::PushTransformData(const TransformDesc& desc)
{
	UpdateData("Transform", desc);
}

void ShaderParameterManager::PushDirectionalLightData(const DirectionalLightDesc& desc)
{
	UpdateData("DirectionalLight", desc);
}

void ShaderParameterManager::PushSpotLightData(const SpotLightDesc& desc)
{
	uint32 index = _spotLightBuffer.spotLightCount++;
	assert(index >= 0 && index < 3);
	_spotLightBuffer.spotLightDescs[index] = desc;
}

void ShaderParameterManager::PushPointLightData(const PointLightDesc& desc)
{
	uint32 index = _pointLightBuffer.pointLightCount++;
	assert(index >= 0 && index < 3);
	_pointLightBuffer.pointLightDescs[index] = desc;
}

void ShaderParameterManager::UpdateAddedLights()
{
	if (_spotLightBuffer.spotLightCount > 0)
		UpdateData("SpotLight", _spotLightBuffer);
	if (_pointLightBuffer.pointLightCount > 0)
		UpdateData("PointLight", _pointLightBuffer);
}

void ShaderParameterManager::CleanUpAddedLightBuffers()
{
	_spotLightBuffer.spotLightCount = 0;
	_pointLightBuffer.pointLightCount = 0;
}

void ShaderParameterManager::PushBoneBuffer(const BoneBuffer& desc)
{
	UpdateData("BoneBuffer", desc);
}

void ShaderParameterManager::PushBoneIndex(const BoneIndex& desc)
{
	UpdateData("BoneIndex", desc);
}

void ShaderParameterManager::PushMaterial(shared_ptr<Material> material)
{
	PushMaterialData(material->GetMaterialDesc());
	_srvBindings = material->GetSRVBindingInfos();
}

void ShaderParameterManager::PushMaterialData(const MaterialDesc& desc)
{
	UpdateData("Material", desc);
}

void ShaderParameterManager::PushEnvLight(shared_ptr<SRVBindingInfo> info)
{
	_envLightInfo = info;
	PushEnvLightOnOff(true);
	_bEnvLightDirty = true;
}

void ShaderParameterManager::PushEnvLightOnOff(bool bOn)
{
	_bEnvLigthOn = bOn;
}

void ShaderParameterManager::PushLightVP(const Matrix& VP)
{
	_shadowDataDesc.lightVP[0] = VP;
	UpdateData("Shadow", _shadowDataDesc);
}

void ShaderParameterManager::PushLightVPs(const vector<Matrix>& VPs)
{
	for (int32 i = 0; i < VPs.size(); ++i)
	{
		_shadowDataDesc.lightVP[i] = VPs[i];
	}

	UpdateData("Shadow", _shadowDataDesc);
}

void ShaderParameterManager::PushPointLightShadowDesc(const array<Matrix, 6>& VPs, Vec3 lightPosition)
{
	for (int32 i = 0; i < 6; ++i)
	{
		_pointShadowDataDesc.lightVP[i] = VPs[i];
	}
	_pointShadowDataDesc.lightPosition = lightPosition;

	UpdateData("PointShadow", _pointShadowDataDesc);
}

//void ShaderParameterManager::UpdateShadowCubeMapVPs(const vector<Matrix>& VPs, uint32 currUsingIndex)
//{
//	for (int32 i = 0; i < VPs.size(); ++i)
//	{
//		_shadowDataDesc.lightVP[i] = VPs[i];
//	}
//	_shadowDataDesc.currUsingCount = currUsingIndex;
//	_shadowDataDesc.bShadowCubeDataLoaded = 1;
//
//	UpdateData("Shadow", _shadowDataDesc);
//}


//void ShaderParameterManager::SetUseShadowCubeTrue()
//{
//	_shadowDataDesc.bShadowCubeDataLoaded = 1;
//}

void ShaderParameterManager::PushShadowMapSRV(shared_ptr<SRVBindingInfo> info)
{
	_shadowMapSrvs.push_back(info);
}

void ShaderParameterManager::PushShadowCubeMapSRV(shared_ptr<SRVBindingInfo> info)
{
	_shadowCubeMapSRV = info;
}


void ShaderParameterManager::CleanUpShadowMapBuffers()
{
	//_shadowDataDesc.currUsingCount = 0;
	//_shadowDataDesc.bShadowCubeDataLoaded = 0;
	_shadowMapSrvs.clear();
}

void ShaderParameterManager::BindCommonResources()
{
	UpdateAddedLights();

	// Shadow Map
	for (int32 i = 0; i < _shadowMapSrvs.size(); ++i)
	{
		CONTEXT->PSSetShaderResources(_shadowMapSrvs[i]->slot + i, 1,
			_shadowMapSrvs[i]->srv.GetAddressOf());
	}

	// TEMP
	if (_shadowCubeMapSRV && _shadowCubeMapSRV->srv)
		CONTEXT->PSSetShaderResources(_shadowCubeMapSRV->slot, 1, _shadowCubeMapSRV->srv.GetAddressOf());

	// other SRVs
	if (_bEnvLightDirty)
	{
		CONTEXT->PSSetShaderResources(_envLightInfo->slot, 1, _envLightInfo->srv.GetAddressOf());
		_bEnvLightDirty = false;
	}
}

void ShaderParameterManager::BindAllDirtyBuffers()
{
	// Constant Buffers
	for (auto& [name, info] : _constbuffers)
	{
		if (!info.dirty)
			continue;

		ComPtr<ID3D11Buffer> comBuffer = info.buffer->GetComPtr();

		if (IsStageVS(info.stage))
			CONTEXT->VSSetConstantBuffers(info.slot, 1, comBuffer.GetAddressOf());

		if (IsStagePS(info.stage))
			CONTEXT->PSSetConstantBuffers(info.slot, 1, comBuffer.GetAddressOf());

		if (IsStageGS(info.stage))
			CONTEXT->GSSetConstantBuffers(info.slot, 1, comBuffer.GetAddressOf());

		info.dirty = false;
	}

	// SRVs From Material
	for (const auto& info : _srvBindings)
	{
		if (IsStageVS(info.stage))
			CONTEXT->VSSetShaderResources(info.slot, 1, info.srv.GetAddressOf());

		if (IsStagePS(info.stage))
			CONTEXT->PSSetShaderResources(info.slot, 1, info.srv.GetAddressOf());
	}
}

void ShaderParameterManager::CleanUpDatasAfterRender()
{
	CleanUpAddedLightBuffers();
	CleanUpShadowMapBuffers();
}
