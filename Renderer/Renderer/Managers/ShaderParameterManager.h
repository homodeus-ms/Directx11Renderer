#pragma once

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Structs/ShaderParameterTypes.h"

#ifndef SHADER_PARAM_MANAGER
#define SHADER_PARAM_MANAGER GET_SINGLE(ShaderParameterManager)
#endif

class Material;

class ShaderParameterManager
{
	DECLARE_SINGLE(ShaderParameterManager);

public:

	void BeginPlay();
	
	template<typename T>
	void RegisterBuffer(const string& bufferName, uint32 slot, EShaderStage stage)
	{
		shared_ptr<ConstantBuffer<T>> buffer = make_shared<ConstantBuffer<T>>();
		buffer->Create();

		BufferBindingInfo info;
		info.slot = static_cast<uint8>(slot);
		info.buffer = buffer;
		info.stage = stage;
			
		if (_constbuffers.find(bufferName) != _constbuffers.end())
		{
			LOG(Error, "BufferName Already Exist!");
			assert(false);
		}

		_constbuffers[bufferName] = info;
	}
	
	void Update();

	// Constant Buffer
	void PushGlobalData(const Matrix& view, const Matrix& projection);
	void PushTransformData(const TransformDesc& desc);
	void PushDirectionalLightData(const DirectionalLightDesc& desc);
	
	void PushSpotLightData(const vector<SpotLightDesc>& descs);
	void PushPointLightData(const vector<PointLightDesc>& descs);

	// TEMP
	void PushSpotLightData(const SpotLightDesc& desc);
	void PushPointLightData(const PointLightDesc& desc);
	SpotLightBuffer _spotLightBuffer;
	PointLightBuffer _pointLightBuffer;
	void UpdateAddedLights();
	void CleanUpAddedLightBuffers();


	void PushBoneBuffer(const BoneBuffer& desc);
	void PushBoneIndex(const BoneIndex& desc);

	// Material
	void PushMaterial(shared_ptr<Material> material);
	void PushMaterialData(const MaterialDesc& desc);
	void PushSRV(const array<SRVBindingInfo, TEXTURE_TYPE_COUNT>& srvs);

	void BindAllDirtyBuffers();

private:

	template<typename T>
	void UpdateData(const string& bufferName, const T& data)
	{
		BufferBindingInfo& info = _constbuffers.at(bufferName);
		uint32 size = sizeof(T);
		shared_ptr<IConstantBuffer> temp = info.buffer;
		shared_ptr<ConstantBuffer<T>> temp2 = static_pointer_cast<ConstantBuffer<T>>(temp);
		temp2->CopyData(data);
		//static_pointer_cast<ConstantBuffer<T>>(info.buffer)->CopyData(data);
		info.dirty = true;
	}

	unordered_map<string, BufferBindingInfo> _constbuffers;
	array<SRVBindingInfo, TEXTURE_TYPE_COUNT> _srvBindings;
	vector<SamplerBindingInfo> _samplerBindings;
};

