#include "VertexShader.h"

bool VertexShader::SetShaderResourceView(std::string name, ID3D11ShaderResourceView* srv)
{
    return false;
}

bool VertexShader::SetSamplerState(std::string name, ID3D11SamplerState* samplerState)
{
    return false;
}

bool VertexShader::CreateProgram(ID3DBlob* shaderBlob)
{
    return false;
}

void VertexShader::SetProgramAndConstantBuffers()
{
}

void VertexShader::Terminate()
{
}

bool VertexShader::SetSRV(std::string& name, ID3D11ShaderResourceView* srv)
{
    return false;
}

bool VertexShader::SetSamplerState(std::string& name, ID3D11SamplerState* samplerState)
{
    return false;
}
