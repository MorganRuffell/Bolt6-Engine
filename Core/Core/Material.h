#pragma once

#include "Texture.h"
#include <array>

/// <summary>
/// 
/// </summary>


class Material
{
public:

	Material(Texture* _Texture, ID3D11SamplerState* _SamplerState);

protected:

	std::array<Texture, 3>  Textures;
	ID3D11SamplerState*		Sampler;
};

