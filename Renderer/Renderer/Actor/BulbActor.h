#pragma once
#include "LightActor.h"

class BasicMesh;
class MaterialBase;

class BulbActor : public LightActor
{
	using Super = LightActor;

public:
	BulbActor(const string& name = "Bulb");
	virtual ~BulbActor();

	virtual void Construct() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

private:
	shared_ptr<BasicMesh> _basicMesh;
	shared_ptr<MaterialBase> _material;
};

