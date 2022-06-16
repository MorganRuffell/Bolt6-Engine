#pragma once

#include "BaseShader.h"

using namespace Microsoft::WRL;

class VertexShader : public BaseShader
{
	VertexShader() = delete;
public:

	VertexShader(_Inout_ ID3D11Device* device, _Inout_ ID3D11DeviceContext* context);
	VertexShader(_Inout_ ID3D11Device* device, _Inout_ ID3D11DeviceContext* context, _Inout_ ID3D11InputLayout* InputLayout, bool PerInstanceCompatible);

	//~VertexShader();

	bool GetPerInstanceCompatible() 
	{ 
		return m_PerInstanceCompatible; 
	}

	bool SetShaderResourceView(std::string name, _Inout_ ID3D11ShaderResourceView* srv);
	bool SetSamplerState(std::string name, _Inout_ ID3D11SamplerState* samplerState);

protected:

	bool								m_PerInstanceCompatible;

	ID3D11InputLayout*					m_inputLayout;

	ID3D11VertexShader*					m_VertexShader;
	ID3D11SamplerState*					m_SamplerState;

public:

	// Inherited via BaseShader
	bool CreateProgram(ID3DBlob* shaderBlob) override;

	void SetProgramAndConstantBuffers() override;

	void Terminate() override;

	bool SetSRV(std::string name, ID3D11ShaderResourceView* srv) override;




	// Inherited via BaseShader
	virtual void SetShaderAndCBs() override;


};