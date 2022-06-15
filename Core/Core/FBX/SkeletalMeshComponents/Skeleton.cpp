#include "Skeleton.h"

Skeleton::Skeleton(Bone2* _RootBone)
{
	SetType(Hierarchy);
	SetTag(EngineObjTag::Asset);

	if (_RootBone == nullptr)
	{
		throw new std::exception("Skeleton cannot be created, there is no root bone!");
	}

	RootBone = _RootBone;
	RootBone->SetBoneIndex(0);
	
	SkeletonRoot = RootBone->BoneTransform;
}

//Constructs a skeleton object at the position
Skeleton::Skeleton(XMFLOAT4X4 Position)
{
	SetType(Hierarchy);
	SetTag(EngineObjTag::Asset);

	std::string Name = "Root";

	RootBone = new Bone2(Name, Position, BoneType::Parent);
	SkeletonRoot = RootBone->BoneTransform;
}

int Skeleton::GetSocketIndex(std::string& JointName, std::vector<Constraint>& Sockets)
{
	return 0;
}

