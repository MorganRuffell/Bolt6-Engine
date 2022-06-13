#pragma once

#include "Texture.h"
#include <array>
#include <Core/Core/Engine/Graphics/PixelShader.h>
#include <Core/Core/Engine/Graphics/VertexShader.h>
#include <cassert>
#include <Core/Core/Engine/Graphics/Accelerator.h>
#include "SamplerState.h"
#include <Core/Core/Object.h>

/// <summary>
/// 
///  Material Class handles all Materials in the Bolt6 test engine
/// 
/// 
/// </summary>

class Material : public Object
{
public:

	Material(TESamplerState* _SamplerState, VertexShader* _vertexShader, PixelShader* _pixelShader, std::string& Name)
	{
		assert(_SamplerState != nullptr);
		assert(_vertexShader != nullptr);
		assert(_pixelShader != nullptr);

		this->m_MaterialPixelShader = _pixelShader;
		this->m_MaterialVertexShader = _vertexShader;
		this->Sampler = _SamplerState;

		SetName(Name);
		SetTag(EngineObjTag::NonEngine);

	}

	Material(TESamplerState* _SamplerState, std::string& Name, Accelerator* Accel)
	{
		assert(_SamplerState != nullptr);

		this->m_MaterialVertexShader = new VertexShader(Accel->Device.Get(), Accel->DeviceContext.Get());
		this->m_MaterialPixelShader = new PixelShader(Accel->Device.Get(), Accel->DeviceContext.Get());
		this->Sampler = _SamplerState;

		SetName(Name);
		SetTag(EngineObjTag::NonEngine);

	}

	Material(std::string& Name, Accelerator* Accel)
	{
		this->m_MaterialVertexShader = new VertexShader(Accel->Device.Get(), Accel->DeviceContext.Get());
		this->m_MaterialPixelShader = new PixelShader(Accel->Device.Get(), Accel->DeviceContext.Get());
		this->Sampler = new TESamplerState(Accel);

		SetName(Name);
		SetTag(EngineObjTag::NonEngine);

	}

public:

	std::string GetName()
	{
		return m_Name;
	}

	void SetName(std::string& Name)
	{
		m_Name = Name;
	}

public:

	Texture* GetDiffuseTexture()
	{
		return DiffuseTexture;
	}

	bool SetDiffuseTexture(Texture* _Texture)
	{
		if (_Texture != nullptr)
		{
			DiffuseTexture = _Texture;
			return true;
		}
		else
		{
			return false;
		}
	}

public:

	Texture* GetNormalTexture()
	{
		return NormalTexture;
	}

	bool SetNormalTexture(Texture* _Texture)
	{
		if (_Texture != nullptr)
		{
			NormalTexture = _Texture;
			return true;
		}
		else
		{
			return false;
		}
	}


protected:

	Texture*				DiffuseTexture;
	Texture*				NormalTexture;

public:

	PixelShader*			m_MaterialPixelShader;
	VertexShader*			m_MaterialVertexShader;

public:

	std::string				m_Name;

protected:

	TESamplerState*		Sampler;
};

