#pragma once
#include <vector>

#include "ShaderStructures.h"
#include <unordered_map>
#include <d3d11shader.h>

/*
	Following Layers of inheritance, this is base graphics program
	It contains the Binary Large Object that holds the Graphics Blob

	It also contains shader resource views, sampler states, Constant buffers and the bespoke
	Index variable.

	This class is not pure virtual, 
*/

class BaseGraphicsProgram
{
protected:

	ID3DBlob* GraphicsBlob;
	ID3D11ShaderReflection* GraphicsReflection;

protected:

	//Holds it's own reference to devices
	ID3D11Device*			device;
	ID3D11DeviceContext*	deviceContext;


	virtual bool CreateProgram(ID3DBlob* shaderBlob) = 0;
	virtual void SetProgramAndConstantBuffers() = 0;
	virtual void Terminate() = 0;

	int ConstantBufferCount;

protected:

	std::vector<SRV*>		shaderResourceViews;
	std::vector<Sampler*>	samplerStates;

protected:

	std::unordered_map<std::string, ConstantBuffer*>	ConstantBufferGrid;
	std::unordered_map<std::string, ShaderVariable>		VarGrid;
};
