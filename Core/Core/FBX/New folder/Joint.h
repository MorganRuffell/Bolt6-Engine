#pragma once

#include <DirectXMath.h>

struct Joint 
{
public:

	int mBoneIndex;
	int mParentIndex;
	const char* mName;

	DirectX::XMFLOAT4X4 mGlobalBindposeInverse = {};
	DirectX::XMFLOAT4X4 mTransform = {};

	int mKeyIndex = 0;

	Joint()
	{
		mBoneIndex = -1;
	}

	~Joint()
	{

	}
};