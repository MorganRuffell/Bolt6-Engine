#pragma once

#include <d3d11.h>
#include <Core/Core/Engine/Graphics/Accelerator.h>

/*
	Sampler State is an object inside of DirectX 11 for managing how we sample textures.

	In all honesty this is completely pointless in the whole getting to MVP thing. 

	But I just think this stuff is so cool...

*/

class TESamplerState
{
public:

	TESamplerState(Accelerator* Accel);
	TESamplerState(Accelerator* Accel, D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE TextureAddressMode);
	TESamplerState(Accelerator* Accel, D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE TextureAddressMode, D3D11_COMPARISON_FUNC ComparisonFunc);

public:

	ID3D11SamplerState* GetSampleState()
	{
		return SampleState;
	}

protected:

	D3D11_SAMPLER_DESC				SampleDesc;
	D3D11_FILTER					SampleFilter;
	D3D11_TEXTURE_ADDRESS_MODE		TextureAddressMode;

	ID3D11SamplerState*				SampleState;


	float							MaxLOD = 400.0f;
	int								MaxANISO = 16;
};

