#pragma once

#include <DirectXMath.h>
//#include "Vertex.h"
#include <d3d11.h>
#include <vector>
#include <Core/Core/Material.h>
#include <Core/Core/Object.h>

class BaseMesh : public Object
{

public:

	ID3D11Buffer* GetVertexBuffer()
	{
		return VertexBuffer;
	}

	ID3D11Buffer* GetIndexBuffer()
	{
		return IndexBuffer;
	}

public:

	Material* GetMaterialatIndex(int index)
	{
		if (index > MeshMaterials.size())
		{
			return nullptr;
		}
		else
		{
			return MeshMaterials.at(index);

		}
	}


	int GetIndexCount()
	{
		return IndexCount;
	}


public:

	std::vector<Material*> MeshMaterials;

protected:

	int				IndexCount;
	int*			Indicies;

protected:

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

};