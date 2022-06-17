#include "BaseShader.h"
#include <d3dcompiler.h>
#include <cassert>
#include <d3d11shader.h>

//Use ID3D11 Shader Reflection objects
#define USE_Shader_Reflection

BaseShader::BaseShader(ID3D11Device* Device, ID3D11DeviceContext* context)
{
	this->device = Device;
	this->deviceContext = context;

	ConstantBufferCount = 0;
	GraphicsBlob = 0;
}

BaseShader::~BaseShader()
{
	if (GraphicsBlob)
	{
		GraphicsBlob->Release();
	}

}

bool BaseShader::LoadShaderFile(LPCTSTR ShaderFile)
{
	assert(ShaderFile != NULL);

	HRESULT LoadResult = D3DReadFileToBlob(ShaderFile, &GraphicsBlob);
	if (LoadResult != S_OK)
	{
		return false;
	}

	Valid = CreateProgram(GraphicsBlob);
	if (!Valid)
	{
		return false;
	}

#ifdef USE_Shader_Reflection

	//D3DReflect(GraphicsBlob->GetBufferPointer(), GraphicsBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&GraphicsReflection);

	GraphicsReflection->GetDesc(&ShaderDesc);

#endif // USE_Shader_Reflection

	ConstantBufferCount = ShaderDesc.ConstantBuffers;
	ConstantBuffers = new ConstantBuffer[ConstantBufferCount];

	//Handle the amount of resoures on a shader.
	int ResourceCount = ShaderDesc.BoundResources;

	for (unsigned int i = 0; i < ResourceCount; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC resourceDesc;

#ifdef USE_Shader_Reflection

		GraphicsReflection->GetResourceBindingDesc(i, &resourceDesc);
#endif

		// Check the type
		switch (resourceDesc.Type)
		{
		case D3D_SIT_TEXTURE:
		{
			// Create the SRV wrapper
			SRV* srv = new SRV();
			srv->BindIndex = resourceDesc.BindPoint;
			srv->Index = (unsigned int)shaderResourceViews.size();	// Raw index

			textureGrid.insert(std::pair<std::string, SRV*>(resourceDesc.Name, srv));
			shaderResourceViews.push_back(srv);
		}
		break;

		case D3D_SIT_SAMPLER: // A sampler resource
		{
			Sampler* samp = new Sampler();
			samp->BindIndex = resourceDesc.BindPoint;
			samp->Index = (unsigned int)samplerStates.size();

			samplerGrid.insert(std::pair<std::string, Sampler*>(resourceDesc.Name, samp));
			samplerStates.push_back(samp);
		}
		break;
		}
	}

	for (int j = 0; j < ConstantBufferCount; j++)
	{
#ifdef USE_Shader_Reflection

		//Find fix or alternate way around without usinf DX11 Reflection
		ID3D11ShaderReflectionConstantBuffer* ConstantBufferObject = GraphicsReflection->GetConstantBufferByIndex(j);
#endif

		D3D11_SHADER_BUFFER_DESC bufferDesc;
		ConstantBufferObject->GetDesc(&bufferDesc);

		// Get the description of the resource binding, so
		// we know exactly how it's bound in the shader
		D3D11_SHADER_INPUT_BIND_DESC bindDesc;
		GraphicsReflection->GetResourceBindingDescByName(bufferDesc.Name, &bindDesc);

		// Set up the buffer and put its pointer in the table
		ConstantBuffers[j].SetBindIndex(bindDesc.BindPoint);
		ConstantBuffers[j].BufferName = bufferDesc.Name;
		ConstantBufferGrid.insert(std::pair<std::string, ConstantBuffer*>(bufferDesc.Name, &ConstantBuffers[j]));

		// Create this constant buffer
		D3D11_BUFFER_DESC newBuffDesc;
		newBuffDesc.Usage = D3D11_USAGE_DEFAULT;
		newBuffDesc.ByteWidth = bufferDesc.Size;
		newBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		newBuffDesc.CPUAccessFlags = 0;
		newBuffDesc.MiscFlags = 0;
		newBuffDesc.StructureByteStride = 0;

		device->CreateBuffer(&newBuffDesc, 0, &ConstantBuffers[j].Resource);

		// Set up the data buffer for this constant buffer
		ConstantBuffers[j].SetSize(bufferDesc.Size);
		ConstantBuffers[j].LocalDataBuffer = new unsigned char[bufferDesc.Size];
		ZeroMemory(ConstantBuffers[j].LocalDataBuffer, bufferDesc.Size);

		for (int k = 0; k < bufferDesc.Variables; k++)
		{
			// Get this variable
			ID3D11ShaderReflectionVariable* var =
				ConstantBufferObject->GetVariableByIndex(k);

			// Get the description of the variable and its type
			D3D11_SHADER_VARIABLE_DESC varDesc;
			var->GetDesc(&varDesc);

			// Create the variable struct
			ShaderVariable varStruct;
			varStruct.IndexInsideConstantBuffer = j;
			varStruct.ByteOffsetInMemory = varDesc.StartOffset;
			varStruct.Size = varDesc.Size;

			// Get a string version
			std::string varName(varDesc.Name);

			// Add this variable to the table and the constant buffer
			VarGrid.insert(std::pair<std::string, ShaderVariable>(varName, varStruct));
			ConstantBuffers[j].Variables.push_back(varStruct);
		}

		GraphicsReflection->Release();
		return true;
	}
}

void BaseShader::SetShader()
{
	if (!Valid)
	{
		return;
	}
	else
	{
		SetShaderAndCBs();
	}
}

void BaseShader::CopyAllBufferData()
{
	assert(deviceContext != nullptr);

	if (!Valid)
	{
		return;
	}

	for (int i = 0; i < ConstantBufferCount; i++)
	{
		deviceContext->UpdateSubresource(ConstantBuffers[i].Resource, 0, 0, ConstantBuffers[i].LocalDataBuffer, 0, 0);
	}
}

void BaseShader::CopyBufferData(unsigned int index)
{
	if (!Valid) return;

	if (index >= this->ConstantBufferCount)
		return;

	ConstantBuffer* cb = &this->ConstantBuffers[index];
	if (!cb) return;

	deviceContext->UpdateSubresource(
		cb->Resource, 0, 0,
		cb->LocalDataBuffer, 0, 0);
}

void BaseShader::CopyBufferData(std::string& bufferName)
{
	// Ensure the shader is valid
	if (!Valid) return;

	// Check for the buffer
	ConstantBuffer* cb = this->FindConstantBuffer(bufferName);
	if (!cb) return;

	// Copy the data and get out
	deviceContext->UpdateSubresource(
		cb->Resource, 0, 0,
		cb->LocalDataBuffer, 0, 0);
}

bool BaseShader::SetData(std::string Name, const void* data, int size)
{
	ShaderVariable* var = FindVariable(Name, size);
	if (var == 0)
		return false;

	// Set the data in the local data buffer
	memcpy(
		ConstantBuffers[var->IndexInsideConstantBuffer].LocalDataBuffer + var->ByteOffsetInMemory,
		data,
		size);

	return true;
}

//This is wrapped in a nullptr, if nothing is found it just returns null
ShaderVariable* BaseShader::FindVariable(std::string& name, int size)
{
	if (VarGrid.size() >= 1)
	{
		std::unordered_map<std::string, ShaderVariable>::iterator result = VarGrid.find(name);

		if (result == VarGrid.end())
		{
			return nullptr;
		}

		ShaderVariable* SearchResult = &(result->second);

		if (size > 0 && SearchResult->Size != size)
		{
			return nullptr;
		}

		return SearchResult;

	}
	else
	{
		return nullptr;
	}
}

ConstantBuffer* BaseShader::FindConstantBuffer(std::string& name)
{
	//If name is invalid
	if (name.size() <= 0)
	{
		return nullptr;
	}

	std::unordered_map<std::string, ConstantBuffer*>::iterator result = ConstantBufferGrid.find(name);

	if (result == ConstantBufferGrid.end())
	{
		return nullptr;
	}
	
	return result->second;
}


void BaseShader::Terminate()
{
	ConstantBufferGrid.clear();
	VarGrid.clear();
	textureGrid.clear();
	samplerGrid.clear();
}

const Sampler* BaseShader::GetSamplerInfo(std::string name)
{
	return nullptr;
}

const Sampler* BaseShader::GetSamplerInfo(unsigned int index)
{
	return nullptr;
}

const SRV* BaseShader::GetShaderResourceViewInfo(std::string& name)
{
	std::unordered_map<std::string, SRV*>::iterator result = textureGrid.find(name);

	if (result == textureGrid.end())
		return nullptr;

	return result->second;
}

const SRV* BaseShader::GetShaderResourceViewInfo(int Index)
{
	if (Index >= shaderResourceViews.size()) return 0;

	return shaderResourceViews[Index];
}

unsigned int BaseShader::GetBufferCount() { return ConstantBufferCount; }

unsigned int BaseShader::GetBufferSize(unsigned int index)
{
	return ConstantBuffers[index].GetBufferSize();
}

const ConstantBuffer* BaseShader::GetBufferInfo(std::string& name)
{
	return FindConstantBuffer(name);
}

const ConstantBuffer* BaseShader::GetBufferInfo(unsigned int index)
{
	return &ConstantBuffers[index];
}
