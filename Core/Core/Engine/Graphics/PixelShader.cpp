#include "PixelShader.h"
#include <cassert>

PixelShader::PixelShader(ID3D11Device* device, ID3D11DeviceContext* context)
	: BaseShader(device, context)
{
	this->shader = 0;
}

PixelShader::~PixelShader()
{
	Terminate();
}

bool PixelShader::CreateShader(ID3DBlob* shaderBlob)
{
	assert(shaderBlob != nullptr);
	assert(device != nullptr);
	
	
	HRESULT pixelShaderResult = device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &shader);


	return (pixelShaderResult == S_OK);
}

void PixelShader::SetShaderAndCBs()
{
	if (!Valid) return;

	deviceContext->PSSetShader(shader, 0, 0);

	for (unsigned int i = 0; i < ConstantBufferCount; i++)
	{
		deviceContext->PSSetConstantBuffers(
			ConstantBuffers[i].GetBindIndex(),
			1,
			&ConstantBuffers[i].Resource);
	}
}

void PixelShader::Terminate()
{
	if (shader)
	{
		shader->Release();
	}
}

bool PixelShader::SetShaderResourceView(std::string& name, ID3D11ShaderResourceView* srv)
{
	assert(deviceContext != nullptr);

	const SRV* srvInfo = GetShaderResourceViewInfo(name);
	if (srvInfo == 0)
		return false;

	deviceContext->PSSetShaderResources(srvInfo->BindIndex, 1, &srv);

	return true;
}

bool PixelShader::SetSamplerState(std::string& name, ID3D11SamplerState* SamplerState)
{
	assert(deviceContext != nullptr);

	const Sampler* sampInfo = GetSamplerInfo(name);
	if (sampInfo == 0)
		return false;

	deviceContext->PSSetSamplers(sampInfo->BindIndex, 1, &SamplerState);

	return true;
}
