#pragma once
#include <vector>
#include "Constraint.h"
#include "Core/Core/AnimObject.h"
#include <Core/Core/FBX/SkeletalMeshComponents/Bone.h>

using namespace DirectX;

struct Skeleton : public AnimObject
{
	Skeleton(Bone2* _RootBone);
	Skeleton(XMFLOAT4X4 Position);


	int GetSocketIndex(_In_ std::string& JointName, _In_ std::vector<Constraint>& Sockets);


public:

	//std::vector<Constraint*> mJoints;

	std::vector<Bone2*> mBones;

	Bone2* GetRootBone()
	{
		assert(RootBone != nullptr);

		return RootBone;
	}


private:

	Bone2*				RootBone;
	XMFLOAT4X4			SkeletonRoot;
};