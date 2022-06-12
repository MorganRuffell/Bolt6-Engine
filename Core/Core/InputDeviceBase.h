#pragma once

#include "Object.h"

class InputDeviceBase : public Object
{

	virtual void DeviceInit() = 0;
	virtual void DeviceUpdate() = 0;
	virtual void DeviceTerminate() = 0;
};

