#pragma once

#include "BaseMesh.h"
#include <Core/Core/Vertex.h>

/*
*	The static mesh is a mesh that does not have any animations,
*	I've dug quite deep into the backing data behind rendering to allow for growth in scope
*	And to improve the scope of use cases.
*/

class StaticMesh : public BaseMesh
{
public:

	StaticMesh(Vertex1* Vertexes, UINT vertexCount, UINT* Indicies, UINT indexCount, Accelerator* Accel);
	StaticMesh(Vertex1* Vertexes, UINT vertexCount, UINT* Indicies, UINT indexCount, Accelerator* Accel, std::string& Name);

	void CalculateTangents(Vertex1* vertices, UINT vertexCount, UINT* indices, UINT indexCount);

	UINT GetVertexCount()
	{
		return VertexCount;
	}

	void SetVertexCount(int _VertexCount)
	{
		VertexCount = _VertexCount;
	}


private:

	int VertexCount;
};

