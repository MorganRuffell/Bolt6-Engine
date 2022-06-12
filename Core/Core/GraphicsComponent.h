#pragma once

#include "EngineComponent.h"

class GraphicsComponent : public EngineComponent
{
	GraphicsComponent()
	{
		SetTag(EngineObjTag::Engine);
	}
};

