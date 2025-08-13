#pragma once
#include "MaterialBase.h"

class IBLMaterial : public MaterialBase
{
	using Super = MaterialBase;

public:
	IBLMaterial();
	virtual ~IBLMaterial();
	IBLMaterial(const IBLMaterial& other);

	virtual void SetEnvMap(shared_ptr<LoadedTexture> envMap) override;
	virtual void SetIBLSpecularMap(shared_ptr<LoadedTexture> iblSpecularMap) override;
	virtual void SetIBLDiffuseMap(shared_ptr<LoadedTexture> iblDiffuseMap) override;
	virtual void SetBRDFMap(shared_ptr<LoadedTexture> brdfMap) override;

	virtual shared_ptr<MaterialBase> Clone() override;
private:
	shared_ptr<LoadedTexture> _envMap;
	shared_ptr<LoadedTexture> _iblSpecularMap;
	shared_ptr<LoadedTexture> _iblDiffuseMap;
	shared_ptr<LoadedTexture> _brdfMap;

	enum { IBL_MATERIAL_SRV_COUNT = 4 };

	//vector<SRVBindingInfo> _srvBindingInfos;
};

