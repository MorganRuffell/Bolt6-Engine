#pragma once
#include <vector>
#include "Joint.h"
#include <Core/Core/FBX/New folder/Bone.h>

using namespace DirectX;

struct Skeleton
{
	Skeleton(Bone* _RootBone);
	Skeleton(XMFLOAT4X4 Position);


public:

	std::vector<Joint*> mJoints;
	std::vector<Bone*> mBones;


private:

	Bone*				RootBone;

	XMFLOAT4X4			SkeletonRoot;
};