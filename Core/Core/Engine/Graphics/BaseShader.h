#pragma once
#include <d3d11.h>
#include <string>
#include <DirectXMath.h>
#include "BaseGraphicsProgram.h"

class BaseShader : public BaseGraphicsProgram
{
public:

	BaseShader(ID3D11Device* Device, ID3D11DeviceContext* context);
	virtual	~BaseShader();


	bool LoadShaderFile(LPCTSTR ShaderFile);
	bool AssertValidity() { return Valid; }

	void SetShader();
	void CopyAllBufferData();
	void CopyBufferData(unsigned int index);
	void CopyBufferData(std::string& bufferName);

public:

	bool SetData(std::string& Name, const void* data, int size);

	//Pure virtuals, this class SHOULD NEVER be instantiated directly
	virtual bool SetSRV(std::string& name, ID3D11ShaderResourceView* srv) = 0;
	virtual bool SetSamplerState(std::string& name, ID3D11SamplerState* samplerState) = 0;
	virtual void SetShaderAndCBs() = 0;

	bool SetMatrix4x4(std::string name, const DirectX::XMFLOAT4X4 data)
	{
		//registers are 16 bytes aligned
		return this->SetData(name, &data, sizeof(float) * 16);
	}

public:

	size_t GetSRVCount() { return textureGrid.size(); };

	const Sampler* GetSamplerInfo(std::string name);
	const Sampler* GetSamplerInfo(unsigned int index);
	size_t GetSamplerCount() { return samplerGrid.size(); }
	
	const SRV* GetShaderResourceViewInfo(std::string& name);
	const SRV* GetShaderResourceViewInfo(int Index);

	// Get data about constant buffers
	unsigned int GetBufferCount();
	unsigned int GetBufferSize(unsigned int index);

	const ConstantBuffer* GetBufferInfo(std::string& name);
	const ConstantBuffer* GetBufferInfo(unsigned int index);

protected:

	std::unordered_map<std::string, SRV*>			textureGrid;
	std::unordered_map<std::string, Sampler*>		samplerGrid;


	D3D11_SHADER_DESC								ShaderDesc;

	ConstantBuffer*									ConstantBuffers;


protected:

	ShaderVariable* FindVariable(std::string& name, int size);
	ConstantBuffer* FindConstantBuffer(std::string& name);

	bool											Valid;

private:

	// Inherited via BaseGraphicsProgram
	//virtual bool CreateProgram(ID3DBlob* shaderBlob) override;

	virtual void Terminate() override;

};

