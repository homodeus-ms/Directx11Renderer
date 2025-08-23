#pragma once

class PSO;
class RenderTexture;

#define POST_EFFECT_SRV_START 30

struct PostEffectParams
{
	// [0] : rendered , [1] : depthMap
	vector<ID3D11ShaderResourceView*> srvs;
	uint32 startSlot = POST_EFFECT_SRV_START;
};

class PostEffect
{
public:
	void Construct();
	void Render(const PostEffectParams& params);
	ComPtr<ID3D11ShaderResourceView> GetSRV();
	PostEffectData* GetPostEffectDataPtr() { return &_postEffectData; }

private:
	void CreateViewport();

	shared_ptr<PSO> _postEffectPSO;
	shared_ptr<RenderTexture> _texture;
	D3D11_VIEWPORT _viewport{};
	PostEffectData _postEffectData{};
};

