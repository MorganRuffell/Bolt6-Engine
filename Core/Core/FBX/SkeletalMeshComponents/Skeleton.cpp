#include "Skeleton.h"

Skeleton::Skeleton(Bone2* _RootBone)
{
	SetType(Hierarchy);
	SetTag(EngineObjTag::NonEngine);

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
	SetType(Hierarchy);
	SetTag(EngineObjTag::NonEngine);

	std::string Name = "Root";

	RootBone = new Bone2(Name, Position, BoneType::Parent);
	SkeletonRoot = RootBone->BoneTransform;
}

int Skeleton::GetSocketIndex(std::string& JointName, std::vector<Socket>& Sockets)
{
	return 0;
}

