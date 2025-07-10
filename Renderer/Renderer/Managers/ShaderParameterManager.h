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
	void PushLightData(const LightDesc& desc);

	template<typename T>
	void PushExtraBufferData(const string& bufferName, const T& data)
	{
		if (_constbuffers.find(bufferName) == _constbuffers.end())
		{
			LOG(Error, "No Such a BufferName");
			assert(false);
		}
		UpdateData(bufferName, data);
	}

	// Material
	void PushMaterial(shared_ptr<Material> material);
	void PushMaterialData(const MaterialDesc& desc);
	void PushSRV(const array<SRVBindingInfo, TEXTURE_TYPE_COUNT>& srvs);

	void BindAll();

private:

	template<typename T>
	void UpdateData(const string& bufferName, const T& data)
	{
		BufferBindingInfo& info = _constbuffers.at(bufferName);
		static_pointer_cast<ConstantBuffer<T>>(info.buffer)->CopyData(data);
		info.dirty = true;
	}

	unordered_map<string, BufferBindingInfo> _constbuffers;
	array<SRVBindingInfo, TEXTURE_TYPE_COUNT> _srvBindings;
	vector<SamplerBindingInfo> _samplerBindings;
};

