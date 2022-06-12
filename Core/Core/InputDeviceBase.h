#pragma once
class InputDeviceBase
{

	virtual void DeviceInit() = 0;
	virtual void DeviceUpdate() = 0;
	virtual void DeviceTerminate() = 0;
};

