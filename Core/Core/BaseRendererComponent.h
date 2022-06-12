#pragma once

#include "EngineComponent.h"

class BaseRendererComponent : EngineComponent
{
protected:

	virtual void Update() = 0;
	virtual void Render() = 0;


};

