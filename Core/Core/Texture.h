#pragma once

/*
	Morgan Ruffell, 2021
*/

#include <d3d11.h>
#include <string>
#include <Core/Core/Object.h>
#include "External/Microsoft/WICTextureLoader.h"
#include <iostream>
#include <assert.h>
#include "Core/Core/Engine/Graphics/Accelerator.h"

/// <summary>
/// Texture class represents an image texture.
/// 
/// Nothing too fancy! ;)
/// </summary>

enum TextureType
{
	Diffuse,
	Normal,
	Roughness
};

struct Resolution
{
	float DimensionX;
	float DimensionY;
};

//Similar to descriptor structs, this functions in a similar way for our textures.
struct TextureContext
{
	LPCWSTR				TextureFilename;
	Resolution			Resolution;
	TextureType			Type;
	bool				AllowOverwriting;
};

class Texture : public Object
{
public:
	Texture(float _DimensionX, float _DimensionY, TextureType _Type, std::string& _Name, Accelerator* _Accel, LPCWSTR File)
	{
		assert(_Accel != nullptr);

		SetTag(EngineObjTag::Asset);

		ResolutionX = _DimensionX;
		ResolutionY = _DimensionY;
		Type = _Type;
		ResourceView = CreateSRV(_Accel, _DimensionX, _DimensionY);
		Name = _Name;

		ID3D11Resource* Resource;

		CreateWICTextureFromFile(_Accel->GetDevice(), _Accel->GetDeviceContext(), File, &Resource, &ResourceView, 4096);

		m_TextureData = reinterpret_cast<ID3D11Texture2D*>(Resource);


		std::cout << "Texture initalization succeeded!" << std::endl;
	}

	Texture(Resolution Res, TextureType _Type, std::string& _Name, Accelerator* _Accel, LPCWSTR File)
	{
		assert(_Accel != nullptr);
		assert(Res.DimensionX != 0 && Res.DimensionY != 0);

		SetTag(EngineObjTag::Asset);

		ResolutionX = Res.DimensionX;
		ResolutionY = Res.DimensionY;
		Type = _Type;
		ResourceView = CreateSRV(_Accel, Res);
		Name = _Name;

		ID3D11Resource* Resource;

		CreateWICTextureFromFile(_Accel->GetDevice(), _Accel->GetDeviceContext(), File, &Resource, &ResourceView, 4096);

		m_TextureData = reinterpret_cast<ID3D11Texture2D*>(Resource);

		std::cout << "Texture initalization succeeded!" << std::endl;

	}

	Texture(TextureContext context, std::string& _Name, Accelerator* _Accel)
	{
		assert(_Accel != nullptr);

		SetTag(EngineObjTag::Asset);

		ResolutionX = context.Resolution.DimensionX;
		ResolutionY = context.Resolution.DimensionY;
		Type = context.Type;
		ResourceView = CreateSRV(_Accel, context.Resolution);
		Name = _Name;

		ID3D11Resource* Resource;
		CreateWICTextureFromFile(_Accel->GetDevice(), _Accel->GetDeviceContext(), context.TextureFilename, &Resource, &ResourceView, 4096);
		m_TextureData = reinterpret_cast<ID3D11Texture2D*>(Resource);

		std::cout << "Texture initalization succeeded!" << std::endl;
	}


public:

	ID3D11ShaderResourceView* CreateSRV(Accelerator* Device, float _DimensionX, float _DimensionY)
	{
		//textureDesc

		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = _DimensionX;
		textureDesc.Height = _DimensionY;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		HRESULT TextureRes = Device->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_TextureData);


		if (SUCCEEDED(TextureRes))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			shaderResourceViewDesc.Format = textureDesc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			HRESULT SRV = Device->GetDevice()->CreateShaderResourceView(m_TextureData, &shaderResourceViewDesc, &ResourceView);

			if (SUCCEEDED(SRV))
			{
				std::cout << "Texture initalization succeeded!" << std::endl;
				return ResourceView;
			}
		}
		else
		{
			throw new std::exception("Texture Failed to initalize");
		}
		

	}

	ID3D11ShaderResourceView* CreateSRV(Accelerator* Device, Resolution res)
	{
		//textureDesc

		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = res.DimensionX;
		textureDesc.Height = res.DimensionY;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		//Fix this as we are using the resource directly and not texture 2D
		HRESULT TextureRes = Device->GetDevice()->CreateTexture2D(&textureDesc, 0, &m_TextureData);

		if (SUCCEEDED(TextureRes))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			shaderResourceViewDesc.Format = textureDesc.Format;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			HRESULT SRV = Device->GetDevice()->CreateShaderResourceView(m_TextureData, &shaderResourceViewDesc, &ResourceView);

			if (SUCCEEDED(SRV))
			{
				std::cout << "Texture initalization succeeded!" << std::endl;
				return ResourceView;
			}
		}
		
		return nullptr;


	}


public:

	TextureType GetType()
	{
		return Type;
	}


	ID3D11Resource* GetResource()
	{
		return m_TextureData;
	}

	~Texture()
	{
		ResourceView->Release();
		m_TextureData->Release();
	}

public:

	ID3D11ShaderResourceView* GetResourceView()
	{
		assert(ResourceView != nullptr);
		return ResourceView;
	}



protected:

	float ResolutionX, ResolutionY;
	TextureType Type;

	std::string Name;

protected:

	ID3D11ShaderResourceView*		ResourceView;

protected:

	ID3D11Texture2D*				m_TextureData;
	D3D11_TEXTURE2D_DESC			textureDesc;
};

