#pragma once
#include <d3d11.h>
#include <string>
#include "BaseGraphicsProgram.h"

class BaseShader : public BaseGraphicsProgram
{
public:

	BaseShader(ID3D11Device* Device, ID3D11DeviceContext* context);
	virtual	~BaseShader();


	bool LoadShaderFile(LPCTSTR ShaderFile);
	bool AssertValidity() {return Valid};

	void SetShader();
	void CopyAllBufferData();
	void CopyBufferData(unsigned int index);
	void CopyBufferData(std::string& bufferName);

public:

	bool SetData(std::string& Name, const void* data, int size);

	//Pure virtuals, this class SHOULD NEVER be instantiated directly
	virtual bool SetSRV(std::string& name, ID3D11ShaderResourceView* srv) = 0;
	virtual bool SetSamplerState(std::string& name, ID3D11SamplerState* samplerState) = 0;

public:

	size_t GetSRVCount() { return textureGrid.size(); };

private:

	std::unordered_map<std::string, SimpleSRV*> textureTable;
	std::unordered_map<std::string, SimpleSampler*> samplerTable;


	bool Valid;

};

