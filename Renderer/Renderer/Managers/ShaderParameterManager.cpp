#include "pch.h"
#include "ShaderParameterManager.h"
#include "Components/CameraComponent.h"
#include "Resource/Material.h"


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

void ShaderParameterManager::PushSpotLightData(const vector<SpotLightDesc>& descs)
{
	SpotLightBuffer buffer;
	uint32 descSize = static_cast<uint32>(descs.size());
	assert(descSize <= MAX_SPOT_LIGHT_COUNT);

	for (uint32 i = 0; i < descSize; ++i)
	{
		buffer.spotLightDescs[i] = descs.at(i);
		buffer.spotLightCount++;
	}
	
	UpdateData("SpotLight", buffer);
}

void ShaderParameterManager::PushPointLightData(const vector<PointLightDesc>& descs)
{
	PointLightBuffer buffer;
	uint32 descSize = static_cast<uint32>(descs.size());
	assert(descSize <= MAX_POINT_LIGHT_COUNT);

	for (uint32 i = 0; i < descSize; ++i)
	{
		buffer.pointLightDescs[i] = descs.at(i);
		buffer.pointLightCount++;
	}

	UpdateData("PointLight", buffer);
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
	PushSRV(material->GetSRVBindingInfos());
}

void ShaderParameterManager::PushMaterialData(const MaterialDesc& desc)
{
	UpdateData("Material", desc);
}

void ShaderParameterManager::PushSRV(const array<SRVBindingInfo, TEXTURE_TYPE_COUNT>& srvs)
{
	_srvBindings = srvs;
}

void ShaderParameterManager::BindAllDirtyBuffers()
{
	UpdateAddedLights();

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

		info.dirty = false;
	}

	for (const auto& info : _srvBindings)
	{
		if (IsStageVS(info.stage))
			CONTEXT->VSSetShaderResources(info.slot, 1, info.srv.GetAddressOf());

		if (IsStagePS(info.stage))
			CONTEXT->PSSetShaderResources(info.slot, 1, info.srv.GetAddressOf());
	}

	CleanUpAddedLightBuffers();
}
