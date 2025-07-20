#pragma once

class Actor;
class ShadowMap;

class RenderManager
{
	DECLARE_SINGLE(RenderManager);

public:
	void SetShadowMap(const shared_ptr<ShadowMap>& shadowMap) { _shadowMap = shadowMap; }
	void Render(vector<shared_ptr<Actor>>& actors);
	
private:
	shared_ptr<ShadowMap> _shadowMap;
};

