#pragma once
#include <string>
#include "GraphicsBufferBase.h"

using namespace Microsoft::WRL;


//Used by shaders stored as part of the material system to store information
//about specific variables inside of Constant buffers
struct ShaderVariable
{
	int ByteOffsetInMemory;
	int Size;
	int IndexInsideConstantBuffer;
};

//Constant Buffers do not change, we have a specific type for these to have a more
//granular control over them
struct ConstantBuffer : public GraphicsBufferBase
{
	std::string BufferName;

	unsigned char* LocalDataBuffer;

	ID3D11Buffer* GetResource()
	{
		Resource.Get();
	}

	int GetBufferSize()
	{
		return BufferSize;
	}

	int GetElementCount()
	{
		return ElementCount;
	}

	int GetBindIndex()
	{
		return BindIndex;
	}

private:

	int BindIndex;
};

struct SRV
{
	unsigned int Index;		
	unsigned int BindIndex; 
};

struct UAV
{
	unsigned int Index;
	unsigned int BindIndex;
};

struct Sampler
{
	unsigned int Index;		
	unsigned int BindIndex; 
};
