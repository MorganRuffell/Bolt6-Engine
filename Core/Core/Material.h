#pragma once

#include "Texture.h"
#include <array>
#include <Core/Core/Engine/Graphics/PixelShader.h>
#include <Core/Core/Engine/Graphics/VertexShader.h>
#include <cassert>
#include <Core/Core/Engine/Graphics/Accelerator.h>
#include "SamplerState.h"
#include <Core/Core/Object.h>

//External File dependencies, this uses Microsoft DirectXTK WIC Texture loader, to load non DDS files
#include "External/Microsoft/WICTextureLoader.h"


/// <summary>
/// 
///  Material Class handles all Materials in the Bolt6 test engine
/// 
/// 
/// </summary>

class Material : public Object
{
public:

	//Does not load texture, you will need to do that seperately.
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

	//Does not load texture, you will need to do that seperately.
	Material(TESamplerState* _SamplerState, std::string& Name, Accelerator* Accel)
	{
		assert(_SamplerState != nullptr);

		this->m_MaterialVertexShader = new VertexShader(Accel->Device.Get(), Accel->DeviceContext.Get());
		this->m_MaterialPixelShader = new PixelShader(Accel->Device.Get(), Accel->DeviceContext.Get());
		this->Sampler = _SamplerState;

		SetName(Name);
		SetTag(EngineObjTag::NonEngine);

	}

	Material(std::string& Name, Accelerator* Accel, TextureContext Diffuse, TextureContext Normal)
	{
		this->m_MaterialVertexShader = new VertexShader(Accel->Device.Get(), Accel->DeviceContext.Get());
		this->m_MaterialPixelShader = new PixelShader(Accel->Device.Get(), Accel->DeviceContext.Get());
		this->Sampler = new TESamplerState(Accel);

		std::string DiffuseTextureName = "T_" + Name + "_BaseColour";
		std::string NormalTextureName = "T_" + Name + "_Normal";

		Texture* DiffuseTex = new Texture(Diffuse, DiffuseTextureName, Accel);
		Texture* NormalTex = new Texture(Normal, NormalTextureName, Accel);

		DiffuseTexture = DiffuseTex;
		NormalTexture = NormalTex;

		SetName(Name);
		SetTag(EngineObjTag::NonEngine);
	}

public:

	void CreateTexture(TextureContext Context, std::string& Name, Accelerator* _Accel)
	{	
		Texture* LTexture = new Texture(Context, Name, _Accel);

		switch (Context.Type)
		{
		case Diffuse:


			if (DiffuseTexture == nullptr)
			{
				DiffuseTexture = LTexture;
			}
			else
			{
				if (Context.AllowOverwriting)
				{
					DiffuseTexture = LTexture;
				}
				else
				{
					std::cout << "Cannot overwrite existing Texture in Diffuse Slot" << std::endl;
				}
			}

			break;

		case Normal:


			if (DiffuseTexture == nullptr)
			{
				NormalTexture = LTexture;
			}
			else
			{
				if (Context.AllowOverwriting)
				{
					DiffuseTexture = LTexture;
				}
				else
				{
					std::cout << "Cannot overwrite existing Texture in Normal Slot" << std::endl;
				}
			}

			break;
		case Roughness:
			std::cout << "Roughness maps are not supported yet!" << std::endl;
			return;
			break;
		default:
			break;
		}
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


	TESamplerState* GetSamplerState()
	{
		return Sampler;
	}

	bool SetSamplerStateOfTexture(Texture* _Texture)
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

	TESamplerState*			Sampler;
};

