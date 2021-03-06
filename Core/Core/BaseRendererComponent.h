#pragma once

#include "EngineComponent.h"
#include <Core/Core/World.h>
#include "GraphicsDevice.h"

class BaseRendererComponent : EngineComponent
{
protected:
	BaseRendererComponent()
	{
		SetTag(EngineObjTag::Engine);
	}

protected:

	virtual void Update(World* world, BaseCamera* Camera, Accelerator* accel) = 0;

	//Overloads for render taking in an accel
	virtual void Render(GraphicsDevice* accel) = 0;
	virtual void Render(GraphicsDevice* accel, int VSyncOpt1, int VSyncOpt2) = 0;
};

