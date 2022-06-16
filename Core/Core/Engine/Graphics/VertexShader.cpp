#include "VertexShader.h"
#include <iostream>
#include <cassert>

VertexShader::VertexShader(ID3D11Device* device, ID3D11DeviceContext* context)
	: BaseShader(device, context)
{
	assert(device != nullptr && context != nullptr);

	this->m_inputLayout = 0;
	this->m_VertexShader = 0;
	this->m_PerInstanceCompatible = false;
}

VertexShader::VertexShader(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11InputLayout* InputLayout, bool PerInstanceCompatible)
	: BaseShader(device, context)
{
	assert(device != nullptr && context != nullptr);
	assert(InputLayout);


	this->m_inputLayout = InputLayout;
	this->m_VertexShader = 0;
	this->m_PerInstanceCompatible = PerInstanceCompatible;
}

bool VertexShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
{
	const SRV* SrvData = GetShaderResourceViewInfo(name);

	if (SrvData == 0)
		return false;

	deviceContext->VSSetShaderResources(SrvData->BindIndex, 1, &srv);

    return true;
}

bool VertexShader::CreateProgram(ID3DBlob* shaderBlob)
{
	//Asserting that this is blob does not contain an already existing shader.
	if (shaderBlob != nullptr)
	{
		return false;
	}

	HRESULT res = device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &m_VertexShader);

	if (res != S_OK)
	{
		return false;
	}

#ifdef USE_SHADER_REFLECTION

	ID3D11ShaderReflection* ShaderReflection;

	D3DReflect(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&refl);

#endif // USE_SHADER_REFLECTION

	D3D11_SHADER_DESC shaderDesc;

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

	for (int i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		//refl->GetInputParameterDesc(i, &paramDesc);

		// Check the semantic name for "_PER_INSTANCE"
		std::string perInstanceStr = "_PER_INSTANCE";
		std::string sem = paramDesc.SemanticName;
		int lenDiff = (int)sem.size() - (int)perInstanceStr.size();
		bool isPerInstance =
			lenDiff >= 0 &&
			sem.compare(lenDiff, perInstanceStr.size(), perInstanceStr) == 0;

		// Fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// Replace anything affected by "per instance" data
		if (isPerInstance)
		{
			elementDesc.InputSlot = 1; // Assume per instance data comes from another input slot!
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}

		// Determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		// Save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	HRESULT InputRes = device->CreateInputLayout(&inputLayoutDesc[0], (int)inputLayoutDesc.size(), shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &m_inputLayout);
	if (InputRes != S_OK)
	{
		std::cout << "Vertex Shader Creation Failed!" << std::endl;
	}
	else
	{
		return true;
	}

	return false;
}

void VertexShader::SetProgramAndConstantBuffers()
{
	assert(Valid == true);
	assert(deviceContext != nullptr);

	deviceContext->IASetInputLayout(m_inputLayout);
	deviceContext->VSSetShader(m_VertexShader, 0, 0);

	for (int i = 0; i < ConstantBufferCount; i++)
	{
		deviceContext->VSSetConstantBuffers(ConstantBuffers[i].GetBindIndex(), 1, &ConstantBuffers[i].Resource);
	}
}

void VertexShader::Terminate()
{
}

bool VertexShader::SetSRV(std::string name, ID3D11ShaderResourceView* srv)
{
	assert(srv != nullptr);

	//const SRV ShaderResourceViewInfo = 
	return false;
}

bool VertexShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
{
	assert(deviceContext != nullptr);

	const Sampler* SamplerInfo = GetSamplerInfo(name);

	if (SamplerInfo)
	{
		return false;
	}

	deviceContext->VSSetSamplers(SamplerInfo->BindIndex, 1, &m_SamplerState);

	return true;
}

void VertexShader::SetShaderAndCBs()
{
	assert(m_inputLayout != nullptr);
	assert(deviceContext != nullptr);

	if (!Valid)
	{
		return;
	}
	else
	{
		deviceContext->IASetInputLayout(m_inputLayout);
		deviceContext->VSSetShader(m_VertexShader, 0, 0);

		for (int i = 0; i < ConstantBufferCount; i++)
		{
			deviceContext->VSSetConstantBuffers(ConstantBuffers[i].GetBindIndex(), 1, &ConstantBuffers[i].Resource);
		}
	}
}
