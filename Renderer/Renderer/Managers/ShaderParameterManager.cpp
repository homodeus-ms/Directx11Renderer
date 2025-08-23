#include "pch.h"
#include "ShaderParameterManager.h"
#include "Components/CameraComponent.h"
#include "Resource/Material/MaterialBase.h"
#include "Resource/Material/IBLMaterial.h"
#include "Resource/Material/Material.h"
#include "Graphics/RenderPass/ShadowMapResources.h"


void ShaderParameterManager::BeginPlay()
{
	// Register Default Constant Buffers
	
	RegisterBuffer<GlobalDesc>("Global", static_cast<uint8>(EConstBufferRegisterNumber::Global), EShaderStage::Both | EShaderStage::GsStage);
	RegisterBuffer<TransformDesc>("Transform", static_cast<uint8>(EConstBufferRegisterNumber::Transform), EShaderStage::VsStage | EShaderStage::GsStage);
	RegisterBuffer<DirectionalLightDesc>("DirectionalLight", static_cast<uint8>(EConstBufferRegisterNumber::DirectionalLight), EShaderStage::PsStage);
	RegisterBuffer<SpotLightBuffer>("SpotLight", static_cast<uint8>(EConstBufferRegisterNumber::SpotLight), EShaderStage::PsStage);
	RegisterBuffer<PointLightBuffer>("PointLight", static_cast<uint8>(EConstBufferRegisterNumber::PointLight), EShaderStage::PsStage);
	
	RegisterBuffer<MaterialDesc>("Material", static_cast<uint8>(EConstBufferRegisterNumber::Material), EShaderStage::Both);
	RegisterBuffer<BoneBuffer>("BoneBuffer", static_cast<uint8>(EConstBufferRegisterNumber::BoneBuffer), EShaderStage::VsStage);
	RegisterBuffer<BoneIndex>("BoneIndex", static_cast<uint8>(EConstBufferRegisterNumber::BoneIndex), EShaderStage::VsStage);
	RegisterBuffer<ShadowDataDesc>("Shadow", static_cast<uint8>(EConstBufferRegisterNumber::ShadowData), EShaderStage::Both);
	RegisterBuffer<PointShadowDataDesc>("PointShadow", static_cast<uint8>(EConstBufferRegisterNumber::PointShadowData), EShaderStage::PsStage | EShaderStage::GsStage);
	RegisterBuffer<ForUIDebugDesc>("ForUIDebug", static_cast<uint8>(EConstBufferRegisterNumber::ForUIDebug), EShaderStage::Both);

	// 특수하게 정의 되어 있는 const Buffers, Global하게 사용하는 cbuffer와 slot번호가 겹쳐서 사용함
	RegisterBuffer<PointShadowDataDesc>("LightIndex", 0, EShaderStage::VsStage);
	RegisterBuffer<FilterData>("FilterData", 0, EShaderStage::PsStage);
	RegisterBuffer<PostEffectData>("PostEffectData", static_cast<uint8>(EConstBufferRegisterNumber::Temporal_Common_Use), EShaderStage::PsStage);
	RegisterBuffer<PostEffectData>("ShaderToyData", static_cast<uint8>(EConstBufferRegisterNumber::Temporal_Common_Use), EShaderStage::PsStage);
	
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
	desc.invP = projection.Invert();

	Matrix invV = view.Invert();
	desc.CameraPosition = { invV._41, invV._42, invV._43 };
	desc.bEnvLightUsing = _bEnvLigthOn ? 1 : 0;

	UpdateData("Global", desc);
}

void ShaderParameterManager::PushGlobalData(const Matrix& view, const Matrix& projection, const Matrix& reflectMatrix)
{
	GlobalDesc desc;

	desc.V = view;
	desc.P = projection;
	desc.VP = reflectMatrix * desc.V * desc.P;

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
	// TODO: 조명에 변화가 없을 경우에는 매번 정보를 올릴 필요가 없음 (constBuffer값 -> 유지)
	// 이 부분을 최적화하려면 각 조명이 static인지 movable 인지 종류를 나눌 필요가 있어보임 
	UpdateData("SpotLight", _spotLightBuffer);
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

void ShaderParameterManager::PushMaterial(shared_ptr<MaterialBase> material)
{
	PushMaterialData(material->GetMaterialDesc());
	const vector<SRVBindingInfo>& srvs = material->GetSRVBindingInfos();
	assert(srvs.size() == _matSRVBindings.size());

	for (uint32 i = 0; i < srvs.size(); ++i)
		_matSRVBindings[i] = srvs[i];

	if (material->IsUsingHeightMap())
	{
		ForUIDebugDesc desc;
		desc.heightScale = material->GetHeightScale();
		PushUIDebugDesc(desc);
	}
}

void ShaderParameterManager::PushMaterialData(const MaterialDesc& desc)
{
	UpdateData("Material", desc);
}

void ShaderParameterManager::PushFilterDataImmediately(const FilterData& data)
{
	UpdateData("FilterData", data);
	// TEMP : 이걸 정리해야하는데?
	BufferBindingInfo& info = _constbuffers["FilterData"];
	ComPtr<ID3D11Buffer> comBuffer = info.buffer->GetComPtr();
	CONTEXT->PSSetConstantBuffers(info.slot, 1, comBuffer.GetAddressOf());
	info.dirty = false;
}

void ShaderParameterManager::PushPostEffectDescImmediately(const PostEffectData& data)
{
	UpdateData("PostEffectData", data);
	
	BufferBindingInfo& info = _constbuffers["PostEffectData"];
	ComPtr<ID3D11Buffer> comBuffer = info.buffer->GetComPtr();
	CONTEXT->PSSetConstantBuffers(info.slot, 1, comBuffer.GetAddressOf());
	info.dirty = false;
}

void ShaderParameterManager::PushShaderToyDataImmediately(const ShaderToyData& data)
{
	UpdateData("ShaderToyData", data);

	BufferBindingInfo& info = _constbuffers["ShaderToyData"];
	ComPtr<ID3D11Buffer> comBuffer = info.buffer->GetComPtr();
	CONTEXT->PSSetConstantBuffers(info.slot, 1, comBuffer.GetAddressOf());
	info.dirty = false;
}

void ShaderParameterManager::PushUIDebugDesc(const ForUIDebugDesc& desc)
{
	UpdateData("ForUIDebug", desc);
}

void ShaderParameterManager::PushIBLInfoOnce(shared_ptr<IBLMaterial> iblMaterial)
{
	const vector<SRVBindingInfo>& infos = iblMaterial->GetSRVBindingInfos();
	_iblSRVBindings[0] = infos[0];
	_iblSRVBindings[1] = infos[1];
	_iblSRVBindings[2] = infos[2];
	_iblSRVBindings[3] = infos[3];

	// Update까지 바로 IBL은 계속 바뀌는 것이 아니므로 한 번만 셋팅
	for (const SRVBindingInfo& info : _iblSRVBindings)
	{
		CONTEXT->PSSetShaderResources(info.slot, 1, info.srv.GetAddressOf());
	}

	PushEnvLightOnOff(true);
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

void ShaderParameterManager::PushCurrentLightVPIndex(uint32 index)
{
	_currentLightVPIndex.index = index;
	UpdateData("LightIndex", _currentLightVPIndex);
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

	// Shadow Cube Map
	if (_shadowCubeMapSRV && _shadowCubeMapSRV->srv)
		CONTEXT->PSSetShaderResources(_shadowCubeMapSRV->slot, 1, _shadowCubeMapSRV->srv.GetAddressOf());

	// other SRVs
	//if (_bEnvLightDirty)
	//{
	//	CONTEXT->PSSetShaderResources(_envLightSpecInfo->slot, 1, _envLightSpecInfo->srv.GetAddressOf());
	//	CONTEXT->PSSetShaderResources(_envLightDiffInfo->slot, 1, _envLightDiffInfo->srv.GetAddressOf());
	//	_bEnvLightDirty = false;
	//}
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
	for (const auto& info : _matSRVBindings)
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
