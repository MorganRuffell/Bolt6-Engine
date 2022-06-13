#pragma once

#include <DirectXMath.h>

#include "Core/Core/AnimObject.h"

struct Socket : public AnimObject
{
public:

	Socket()
	{
		SetType(Joint);
		SetTag(EngineObjTag::NonEngine);

		mBoneIndex = -1;
	}

	~Socket()
	{

	}

public:





protected:

	int mBoneIndex;
	int mParentIndex;

	std::string m_SocketName;

	DirectX::XMFLOAT4X4 mGlobalBindposeInverse = {};
	DirectX::XMFLOAT4X4 mTransform = {};

	int mKeyIndex = 0;

};