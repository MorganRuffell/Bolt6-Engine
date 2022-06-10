#pragma once
#include <d3d11.h>
#include <string>

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

class Texture
{
public:

	Texture(float _DimensionX, float _DimensionY, TextureType _Type, ID3D11ShaderResourceView* _ResourceView, std::string& _Name )
	{
		ResolutionX = _DimensionX;
		ResolutionY = _DimensionY;
		Type = _Type;
		ResourceView = _ResourceView;
		Name = _Name.c_str();
	}
	
	~Texture()
	{
		ResourceView->Release();
	}

	TextureType GetType()
	{
		return Type;
	}


private:

	float ResolutionX, ResolutionY;
	TextureType Type;

	const char* Name;

protected:

	ID3D11ShaderResourceView* ResourceView;

};

