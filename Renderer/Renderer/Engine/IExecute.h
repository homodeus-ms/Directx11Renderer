#pragma once

class IExecute
{
public:
	virtual void Construct() abstract;
	virtual void BeginPlay() abstract;
	virtual void Tick() abstract;
	virtual void Render() abstract;
};

