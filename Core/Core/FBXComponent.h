#pragma once

#include "ImportComponent.h"

class FBXComponent : public ImportComponent
{
	FBXComponent()
	{

	}

	~FBXComponent()
	{

	}

protected:

	bool Result;

	virtual void InitalizeComponent();
	virtual bool TerminateComponent();
};

