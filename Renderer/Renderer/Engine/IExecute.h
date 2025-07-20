#pragma once

class IExecute
{
public:
	virtual ~IExecute() {}

	virtual void Construct() abstract;
	virtual void BeginPlay() abstract;
	virtual void Tick() abstract;
	virtual void Render() abstract;

};

