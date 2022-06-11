#pragma once

#include "BaseShader.h"

class PixelShader : public BaseShader
{
public:
	PixelShader(ID3D11Device* device, ID3D11DeviceContext* context);
	~PixelShader();

	ID3D11PixelShader* GetPixelShader() { return shader; }

protected:

	ID3D11PixelShader* shader;

protected:

	bool CreateShader(ID3DBlob* shaderBlob);
	void SetShaderAndCBs() override;
	void Terminate();
	
protected:

	bool SetShaderResourceView(std::string& name, ID3D11ShaderResourceView* srv);
	bool SetSamplerState(std::string& name, ID3D11SamplerState* SamplerState);

	// Inherited via BaseShader
	virtual bool CreateProgram(ID3DBlob* shaderBlob) override;
	virtual void SetProgramAndConstantBuffers() override;
	virtual bool SetSRV(std::string& name, ID3D11ShaderResourceView* srv) override;
};
