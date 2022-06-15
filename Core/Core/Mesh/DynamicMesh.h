#pragma once

#include <Core/Core/Mesh/BaseMesh.h>
#include <Core/Core/FBX/SkeletalMeshComponents/Skeleton.h>
#include <Core/Core/Engine/Animation/AnimationStructures.h>


class DynamicMesh : public BaseMesh
{
public:

	DynamicMesh(Skeleton* _skl, AnimationSequence* _Seq)
	{
		SetTag(EngineObjTag::NonEngine);
		m_skeleton = _skl;
		m_Animation = _Seq;
	}

	DynamicMesh(Skeleton* _skl)
	{
		SetTag(EngineObjTag::NonEngine);
		m_skeleton = _skl;
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

public:

	AnimationSequence* GetSequence()
	{
		return m_Animation;
	}

protected:

	Skeleton*					m_skeleton;
	AnimationSequence*			m_Animation;
};
