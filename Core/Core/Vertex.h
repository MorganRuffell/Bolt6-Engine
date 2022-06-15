#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Vertex
{
public:
	XMFLOAT3 Position;		//0-byte offset
};

class Vertex1 : public Vertex
{
public:

	Vertex1()
	{

	}

	Vertex1(DirectX::XMFLOAT3 _Position, DirectX::XMFLOAT3 _Normal, DirectX::XMFLOAT2 _UV)
	{
		Position = _Position;
		Normal = _Normal;
		UV = _UV;
	}

public:
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Tangent;
};

class Vertex3 : public Vertex1
{
public:
	DirectX::XMFLOAT4 Boneids{ 0,0,0,0 };
	DirectX::XMFLOAT4 Weights{ 0,0,0,0 };
};