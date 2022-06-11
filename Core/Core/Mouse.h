#pragma once

#include "InputDeviceBase.h"

enum Movement
{

};

class Mouse : public InputDeviceBase
{
	virtual void DeviceInit() override;
	virtual void DeviceUpdate() override;
	virtual void DeviceTerminate() override;
};

