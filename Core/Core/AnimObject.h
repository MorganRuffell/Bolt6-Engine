#pragma once

#include "Object.h"


enum AnimObjectType
{
	Hierarchy,
	Joint,
	Bone
};

class AnimObject : public Object
{
protected:

	AnimObjectType GetType()
	{
		return ObjectType;
	}

	void SetType(AnimObjectType TagType)
	{
		ObjectType = TagType;
	}

protected:

	AnimObjectType			ObjectType;

};