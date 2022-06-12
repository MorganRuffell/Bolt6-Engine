#pragma once
#include <vector>
#include "Joint.h"
#include "Core/Core/AnimObject.h"
#include <Core/Core/FBX/New folder/Bone.h>

using namespace DirectX;

struct Skeleton : public AnimObject
{
	Skeleton(Bone2* _RootBone);
	Skeleton(XMFLOAT4X4 Position);


public:

	std::vector<Socket*> mJoints;

	std::vector<Bone2*> mBones;


private:

	Bone2*				RootBone;

	XMFLOAT4X4			SkeletonRoot;
};