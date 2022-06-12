#pragma once

#include "ImportComponent.h"

class FBXComponent : public ImportComponent
{
public:
	FBXComponent()
	{
		SetTag(EngineObjTag::Engine);

	}

	~FBXComponent()
	{

	}

protected:

	bool Result;

	virtual void InitalizeComponent();
	virtual bool TerminateComponent();
};

