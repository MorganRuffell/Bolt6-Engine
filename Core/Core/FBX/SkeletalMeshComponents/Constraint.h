#pragma once

#include <DirectXMath.h>

#include "Core/Core/AnimObject.h"


/*
*	Constraints are assigned to bones, they are paired together in the skeleton.
*/

struct Constraint : public AnimObject
{
public:

	Constraint()
	{
		SetType(Joint);
		SetTag(EngineObjTag::NonEngine);

		mBoneIndex = -1;
	}

	~Constraint()
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