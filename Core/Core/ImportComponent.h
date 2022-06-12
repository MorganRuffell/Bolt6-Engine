#pragma once

#include "EngineComponent.h"
#include "Object.h"

namespace ImportComponentState
{
	enum ImportState
	{
		Loading,
		Importing,
		Constructing,
		Ending,
		Error,
		Complete
	};
}

class ImportComponent : public EngineComponent
{
protected:

	float BlendWeight;


protected:

	float BeginningTimeImport	= 0.0f;
	float FinalImportTime		= 0.0f;

public:






protected:

	ImportComponentState::ImportState State;
};

