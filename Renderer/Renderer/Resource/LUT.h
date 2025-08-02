#pragma once
#include "ResourceBase.h"
#include "Structs/ShaderParameterTypes.h"

class Texture3D;

class LUT : public ResourceBase
{
	using Super = ResourceBase;

public:
	LUT();
	virtual ~LUT();

	virtual void Load(const wstring& path) override;
	vector<Vec3>& GetLUTData() { return _LUTData; }
	shared_ptr<SRVBindingInfo> GetSRVBindingInfo();

private:
	vector<Vec3> _LUTData{};
	uint32 _LUTSize{};
	shared_ptr<Texture3D> _LUTTexture;
};

