#pragma once

#include <DirectXMath.h>
#include <string>
#include <map>

using namespace DirectX;

/* Inspired by Epic's Unreal Engine, constrain the translation
   to the skeleton or to the animation */

enum BoneTranslationMode
{
	Animation,
	Skeleton,
};

/*
	Bones follow a structure similar to that of a tree data structure,
	this level of granularity could be argued to be superflous, but I think it's fun!
*/

enum BoneType
{
	Parent,
	Leaf,
	Branch
};

class Bone
{
	//Constructor for creating a new bone relative to it's parent
	Bone(std::string& Name, XMFLOAT4X4 OffsetPosition, Bone* Parent)
	{
		SetName(Name);

		//Rewrite the maths on this -- This isn't accurate enough
		XMFLOAT4X4 Position = Parent->BoneTransform;

		ApplyTranslationFromParent(Position, OffsetPosition);

		BoneTransform = Position;
		TranslationMode = Skeleton;
		this->Parent = Parent;
		
		if (this->NumberOfChildren = 0)
		{
			TypeOfBone = BoneType::Leaf;
		}
		else
		{
			TypeOfBone = BoneType::Branch;
		}
	}

	//Constructor for creating a new bone relative to it's parent, name is ommited
	Bone(XMFLOAT4X4 OffsetPosition, Bone* Parent)
	{
		std::string Name = Parent->GetName() + "_" + std::to_string(Parent->NumberOfChildren + 1);

		SetName(Name);

		//Rewrite the maths on this -- This isn't accurate enough
		XMFLOAT4X4 Position = Parent->BoneTransform;

		ApplyTranslationFromParent(Position, OffsetPosition);

		BoneTransform = Position;
		TranslationMode = Skeleton;
		this->Parent = Parent;

		if (this->NumberOfChildren = 0)
		{
			TypeOfBone = BoneType::Leaf;
		}
		else
		{
			TypeOfBone = BoneType::Branch;
		}
	}

	
	//Constructor for creating a parent bone
	Bone(std::string& Name, XMFLOAT4X4 Position, BoneType Type)
	{
		if (Type == BoneType::Parent)
		{
			SetName(Name);
			BoneTransform = Position;
			TranslationMode = Skeleton;
			TypeOfBone = BoneType::Parent;
			NumberOfChildren = 0;
			Parent == nullptr;
		}
		else
		{
			throw new std::exception("Wrong constructor used for bone, this must be used for parent bones only.");
		}
	}

	
public:

	void SetTranslationMode(BoneTranslationMode Mode)
	{
		TranslationMode = Mode;
	}
	
	BoneTranslationMode GetTranslationMode()
	{
		return TranslationMode;
	}

public:

	void RemoveParent()
	{
		Parent = nullptr;
	}

	void AddParent(Bone* NewParent)
	{
		if (GetParentBone() == nullptr)
		{
			Parent = NewParent;
		}
		else
		{
			RemoveParent();
			Parent = NewParent;
		}
	}

public:

	std::string GetName()
	{
		return Name;
	}

	void SetName(std::string& _name)
	{
		Name = _name;
	}

public:

	Bone* GetParentBone()
	{
		if (Parent != nullptr)
		{
			return Parent;
		}
		else
		{
			return nullptr;
		}
	}

	Bone* GetChildBoneByName(std::string& Name)
	{
		for (auto& element : Children)
		{
			if (element->GetName() == Name)
			{
				return element;
			}
		}
		
		//No child bone found by name
		return nullptr;
	}

protected:

	void ApplyTranslationFromParent(XMFLOAT4X4& Position, XMFLOAT4X4& OffsetPosition)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				Position.m[i][j] += OffsetPosition.m[i][j];
			}
		}
	}

public:

	bool AddChild(Bone* Child)
	{
		assert(Child != nullptr);

		if (NumberOfChildren < MaximumNumberOfChildBones)
		{
			Children.push_back(Child);
			NumberOfChildren++;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool AddChild(std::string& Name, std::map<std::string, Bone*> Collection)
	{
		if (Collection.contains(Name) && Collection.at(Name) != nullptr)
		{
			Bone* NewChild = Collection.at(Name);

			if (NumberOfChildren < MaximumNumberOfChildBones)
			{
				Children.push_back(NewChild);
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	bool AddChild(int index, std::vector<Bone*> Collection)
	{
		if (index > Collection.size())
		{
			Bone* NewChild = Collection.at(index);

			if (NumberOfChildren < MaximumNumberOfChildBones)
			{
				Children.push_back(NewChild);
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

public:

	bool RemoveAllChildren()
	{
		NumberOfChildren = 0;

		for (Bone* Element : Children)
		{
			delete(Element);
		}

		if (Children.size() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

public:

	DirectX::XMFLOAT4X4		BoneTransform = {};
	DirectX::XMFLOAT4X4		InvBoneTransform = {};

private:

	BoneTranslationMode		TranslationMode;
	BoneType				TypeOfBone;

private:

	Bone*					Parent;

	std::vector<Bone*>		Children;

	int						NumberOfChildren;
	int						MaximumNumberOfChildBones = 25;

	std::string				Name;
};
