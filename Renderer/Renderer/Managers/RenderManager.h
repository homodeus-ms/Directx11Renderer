#pragma once

class Actor;

class RenderManager
{
	DECLARE_SINGLE(RenderManager);

public:
	void Render(vector<shared_ptr<Actor>>& actors);
};

