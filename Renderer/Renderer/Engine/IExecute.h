#pragma once

class IExecute
{
public:
	virtual void BeginPlay() abstract;
	virtual void Tick() abstract;
	virtual void Render() abstract;
};

