#include "pch.h"
#include "ShaderParameterManager.h"
#include "Components/CameraComponent.h"
#include "Resource/Material.h"


void ShaderParameterManager::BeginPlay()
{
	// Register Default Constant Buffers
	RegisterBuffer<GlobalDesc>("Global", 0, EShaderStage::VsStage);
	RegisterBuffer<TransformDesc>("Transform", 1, EShaderStage::VsStage);
	RegisterBuffer<LightDesc>("Light", 2, EShaderStage::PsStage);
	RegisterBuffer<MaterialDesc>("Material", 3, EShaderStage::PsStage);
}

void ShaderParameterManager::Update()
{
	PushGlobalData(CameraComponent::S_MatView, CameraComponent::S_MatProjection);
}

void ShaderParameterManager::PushGlobalData(const Matrix& view, const Matrix& projection)
{
	GlobalDesc desc;
	desc.V = view;
	desc.P = projection;
	desc.VP = desc.V * desc.P;
	UpdateData("Global", desc);
}

void ShaderParameterManager::PushTransformData(const TransformDesc& desc)
{
	UpdateData("Transform", desc);
}

void ShaderParameterManager::PushLightData(const LightDesc& desc)
{
	UpdateData("Light", desc);
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

void ShaderParameterManager::BindAll()
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

		// TODO: 

		info.dirty = false;
	}

	for (const auto& info : _srvBindings)
	{
		if (IsStageVS(info.stage))
			CONTEXT->VSSetShaderResources(info.slot, 1, info.srv.GetAddressOf());

		if (IsStagePS(info.stage))
			CONTEXT->PSSetShaderResources(info.slot, 1, info.srv.GetAddressOf());
	}
}
