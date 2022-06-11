#include "SamplerState.h"
#include <cassert>
#include <iostream>

TESamplerState::TESamplerState(Accelerator* Accel)
{
	assert(Accel != nullptr);

	SampleDesc = {};
	SampleFilter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	TextureAddressMode = D3D11_TEXTURE_ADDRESS_WRAP;

	SampleDesc.Filter = SampleFilter;

	SampleDesc.AddressU = TextureAddressMode;
	SampleDesc.AddressV = TextureAddressMode;
	SampleDesc.AddressW = TextureAddressMode;

	SampleDesc.MaxLOD = MaxLOD;
	SampleDesc.MaxAnisotropy = MaxANISO;

	SampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT SamplerStateRes = Accel->Device->CreateSamplerState(&SampleDesc, &SampleState);

	if (SamplerStateRes != S_OK)
	{
		std::cout << "Sampler State Creation Failed!" << std::endl;
	}
	else
	{
		std::cout << "Sampler State Creation Succeeded!" << std::endl;
	}
}

TESamplerState::TESamplerState(Accelerator* Accel, D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE TextureAddressMode)
{
	SampleDesc.Filter = Filter;

	SampleDesc.AddressU = TextureAddressMode;
	SampleDesc.AddressV = TextureAddressMode;
	SampleDesc.AddressW = TextureAddressMode;

	SampleDesc.MaxAnisotropy = MaxANISO;
	SampleDesc.MaxLOD = MaxLOD;

	SampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT SamplerStateRes = Accel->Device->CreateSamplerState(&SampleDesc, &SampleState);

	if (SamplerStateRes != S_OK)
	{
		std::cout << "Sampler State Creation Failed!" << std::endl;
	}
	else
	{
		std::cout << "Sampler State Creation Succeeded!" << std::endl;
	}
}

TESamplerState::TESamplerState(Accelerator* Accel, D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE TextureAddressMode, D3D11_COMPARISON_FUNC ComparisonFunc)
{
	SampleDesc.Filter = Filter;

	SampleDesc.AddressU = TextureAddressMode;
	SampleDesc.AddressV = TextureAddressMode;
	SampleDesc.AddressW = TextureAddressMode;

	SampleDesc.MaxAnisotropy = MaxANISO;
	SampleDesc.MaxLOD = MaxLOD;

	SampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT SamplerStateRes = Accel->Device->CreateSamplerState(&SampleDesc, &SampleState);

	if (SamplerStateRes != S_OK)
	{
		std::cout << "Sampler State Creation Failed!" << std::endl;
	}
	else
	{
		std::cout << "Sampler State Creation Succeeded!" << std::endl;
	}
}
