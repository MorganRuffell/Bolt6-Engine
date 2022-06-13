#pragma once

#include "Mesh/BaseMesh.h"

class StaticMesh : public BaseMesh
{
	StaticMesh()
	{
		SetTag(EngineObjTag::NonEngine);
	}



};

