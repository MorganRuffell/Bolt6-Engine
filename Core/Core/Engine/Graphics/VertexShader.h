#pragma once

#include "BaseShader.h"

using namespace Microsoft::WRL;

class VertexShader : public BaseShader
{
public:
	VertexShader(ID3D11Device* device, ID3D11DeviceContext* context);
	VertexShader(ID3D11Device* device, ID3D11DeviceContext* context, ID3D11InputLayout* InputLayout, bool PerInstanceCompatible);

	//~VertexShader();

	bool GetPerInstanceCompatible() 
	{ 
		return m_PerInstanceCompatible; 
	}

	bool SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv);
	bool SetSamplerState(std::string name, ID3D11SamplerState* samplerState);

protected:

	bool								m_PerInstanceCompatible;

	ComPtr<ID3D11InputLayout>			m_inputLayout;
	ComPtr<ID3D11VertexShader>			m_VertexShader;

public:

	// Inherited via BaseShader
	bool CreateProgram(ID3DBlob* shaderBlob) override;

	void SetProgramAndConstantBuffers() override;

	void Terminate() override;

	bool SetSRV(std::string& name, ID3D11ShaderResourceView* srv) override;

	bool SetSamplerState(std::string& name, ID3D11SamplerState* samplerState) override;


};