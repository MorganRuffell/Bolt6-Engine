#include "BaseShader.h"
#include <d3dcompiler.h>
#include <cassert>
#include <d3d11shader.h>


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
	assert(GraphicsBlob != NULL);

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

	D3DReflect(GraphicsBlob->GetBufferPointer(), GraphicsBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&GraphicsReflection);

	GraphicsReflection->GetDesc(&ShaderDesc);

	ConstantBufferCount = ShaderDesc.ConstantBuffers;

	return false;
}

void BaseShader::SetShader()
{
}

void BaseShader::CopyAllBufferData()
{
}

void BaseShader::CopyBufferData(unsigned int index)
{
}

void BaseShader::CopyBufferData(std::string& bufferName)
{
}

bool BaseShader::SetData(std::string& Name, const void* data, int size)
{
	return false;
}

bool BaseShader::CreateProgram(ID3DBlob* shaderBlob)
{
	return false;
}

void BaseShader::SetProgramAndConstantBuffers()
{
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

unsigned int BaseShader::GetBufferCount()
{
	return 0;
}

unsigned int BaseShader::GetBufferSize(unsigned int index)
{
	return 0;
}

const Sampler* BaseShader::GetBufferInfo(std::string name)
{
	return nullptr;
}

const Sampler* BaseShader::GetBufferInfo(unsigned int index)
{
	return nullptr;
}
