#pragma once

#include "BaseMesh.h"
#include <Core/Core/Vertex.h>


class StaticMesh : public BaseMesh
{
public:

	StaticMesh(Vertex2* Vertexes, int vertexCount, int* Indicies, int indexCount, Accelerator* Accel);

	void CalculateTangents(Vertex2* vertices, int vertexCount, int* indices, int indexCount);




private:


};

