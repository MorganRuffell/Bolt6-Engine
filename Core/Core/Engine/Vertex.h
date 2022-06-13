#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Vertex
{
	XMFLOAT3 Position;		//0-byte offset
};

struct Vertex1 : public Vertex
{
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Tangent;
};

class Vertex2
{
	XMFLOAT2 Tex0;			//24-byte offset
	XMFLOAT2 Tex1;			//32-byte offset
};

class Vertex3
{
	DirectX::XMFLOAT4 Position{ 0,0,0,0 };	    // The position of the vertex
	DirectX::XMFLOAT3 Normal{ 0,0,0 };

	DirectX::XMFLOAT4 Boneids{ 0,0,0,0 };
	DirectX::XMFLOAT4 Weights{ 0,0,0,0 };

	DirectX::XMFLOAT2 UV{ 0,0 };
	DirectX::XMFLOAT3 Tangent{ 0,0 ,0 };
};