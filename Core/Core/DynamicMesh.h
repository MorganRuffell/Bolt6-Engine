#pragma once

#include "Mesh/BaseMesh.h"
#include <Core/Core/FBX/New folder/Skeleton.h>

class DynamicMesh : public BaseMesh
{
public:

	DynamicMesh()
	{

	}

	~DynamicMesh()
	{

	}

public:

	Skeleton* GetSkeleton()
	{
		return m_skeleton;
	}

	void SetSkeleton(Skeleton* Skl)
	{
		m_skeleton = Skl;
	}

protected:

	Skeleton*	m_skeleton;
};

