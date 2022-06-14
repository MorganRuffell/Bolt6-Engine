#pragma once

#include <string>

/*
	Object is the base class that everything in the solution
	inherits from it contains tag information about what each
	object is.

	Objects that are tagged Engine are to be used for engine,
	objects that are tagged Non-Engine are to be used as assets
	or elements that are drawn in the world.

	This is sooo much fun, I could do this forever!
*/

namespace EngineObjTag
{
	enum ObjectTag
	{
		Engine,
		NonEngine,
	};
}


class Object
{
public:
	Object() { }

	~Object()
	{
	}

	std::string GetName()
	{
		return ObjectName;
	}

	void SetName(std::string Name)
	{
		ObjectName = Name;
	}

public:

	EngineObjTag::ObjectTag GetTag()
	{
		return Tag;
	}

	void SetTag(EngineObjTag::ObjectTag TagType)
	{
		Tag = TagType;
	}

protected:

	std::string						ObjectName;
	EngineObjTag::ObjectTag			Tag;
};


