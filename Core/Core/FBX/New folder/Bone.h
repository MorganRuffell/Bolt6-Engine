#pragma once

#include <DirectXMath.h>

struct Bone
{
	DirectX::XMFLOAT4X4 BoneTransform = {};
	DirectX::XMFLOAT4X4 InvBoneTransform = {};
};
