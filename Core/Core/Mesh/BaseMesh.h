#pragma once

#include <DirectXMath.h>
#include "Vertex.h"
#include <d3d11.h>

class BaseMesh
{
protected:

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

public:

	ID3D11Buffer* GetVertexBuffer()
	{
		return VertexBuffer;
	}

	ID3D11Buffer* GetIndexBuffer()
	{
		return IndexBuffer;
	}

};