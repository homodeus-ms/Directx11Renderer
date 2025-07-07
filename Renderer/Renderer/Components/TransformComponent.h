#pragma once
#include "Component.h"

class TransformComponent : public Component
{
	using Super = Component;

public:
	TransformComponent();
	virtual ~TransformComponent();
};

