#include "Skeleton.h"

Skeleton::Skeleton(Bone* _RootBone)
{
	if (_RootBone == nullptr)
	{
		throw new std::exception("Skeleton cannot be created, there is no root bone!");
	}

	RootBone = _RootBone;

	SkeletonRoot = RootBone->BoneTransform;
}

//Constructs a skeleton object at the position
Skeleton::Skeleton(XMFLOAT4X4 Position)
{
	RootBone = new Bone();


}

