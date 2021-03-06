#pragma once

#include <wrl.h>
#include <d3d11.h>
#include "Core/Core/Object.h"

using namespace Microsoft::WRL;

//Accelerators are your discrete graphics card, or
//literally anything else. I'm using it's 'Proper name' because it sounds cooler
//I came up with this idea from accelerated mass paralellism.

class Accelerator : public Object
{
public:
	Accelerator()
	{
		SetTag(EngineObjTag::Engine);
	}

public:

	ID3D11Device* GetDevice()
	{
		return Device.Get();
	}

	ID3D11DeviceContext* GetDeviceContext()
	{
		return DeviceContext.Get();
	}

	bool TerminateAccelerator()
	{
		if (DeviceContext)
		{
			DeviceContext->Release();
		}

		if (Device)
		{
			Device->Release();
		}

		return true;
	}

public:

	ComPtr<ID3D11Device>			Device;
	ComPtr<ID3D11DeviceContext>		DeviceContext;
};

//Here for when the test is over and I keep experimenting with this thing... Because this is cool.
class VideoAccelerator
{

};