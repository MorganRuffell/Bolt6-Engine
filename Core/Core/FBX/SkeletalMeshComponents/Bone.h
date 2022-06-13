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
	Parental,
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

class Bone2
{

public:

	//Constructor to be used ONLY for fbx import process
	Bone2()
	{

	}

	//Constructor for creating a new bone relative to it's parent
	Bone2(std::string& Name, XMFLOAT4X4 OffsetPosition, Bone2* Parent)
	{
		SetName(Name);

		//Rewrite the maths on this -- This isn't accurate enough
		XMFLOAT4X4 Position = m_ParentBone->BoneTransform;

		ApplyTranslationFromParent(Position, OffsetPosition);

		BoneTransform = Position;
		TranslationMode = Parental;
		//this->m_ParentBone = m_ParentBone;

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
	Bone2(XMFLOAT4X4 OffsetPosition, Bone2* Parent)
	{
		std::string Name = m_ParentBone->GetName() + "_" + std::to_string(m_ParentBone->NumberOfChildren + 1);

		SetName(Name);

		//Rewrite the maths on this -- This isn't accurate enough
		XMFLOAT4X4 Position = m_ParentBone->BoneTransform;

		ApplyTranslationFromParent(Position, OffsetPosition);

		BoneTransform = Position;
		TranslationMode = Parental;
		this->m_ParentBone = Parent;

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
	Bone2(std::string& Name, XMFLOAT4X4 Position, BoneType Type)
	{
		if (Type == BoneType::Parent)
		{
			SetName(Name);
			BoneTransform = Position;
			TranslationMode = Parental;
			TypeOfBone = BoneType::Parent;
			NumberOfChildren = 0;
			m_ParentBone = nullptr;
		}
		else
		{
			throw new std::exception("Wrong constructor used for bone, this must be used for parent bones only.");
		}
	}

	Bone2(const char* Name, XMFLOAT4X4 Position, BoneType Type)
	{
		if (Type == BoneType::Parent)
		{
			SetName(Name);
			BoneTransform = Position;
			TranslationMode = Parental;
			TypeOfBone = BoneType::Parent;
			NumberOfChildren = 0;
			m_ParentBone = nullptr;
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
		m_ParentBone = nullptr;
	}

	void AddParent(Bone2* NewParent)
	{
		if (GetParentBone() == nullptr)
		{
			m_ParentBone = NewParent;
		}
		else
		{
			RemoveParent();
			m_ParentBone = NewParent;
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

	void SetName(const char* _name)
	{
		Name = _name;
	}

public:

	Bone2* GetParentBone()
	{
		if (m_ParentBone != nullptr)
		{
			return m_ParentBone;
		}
		else
		{
			return nullptr;
		}
	}

	Bone2* GetChildBoneByName(std::string& Name)
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

	bool AddChild(Bone2* Child)
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

	bool AddChild(std::string& Name, std::map<std::string, Bone2*> Collection)
	{
		if (Collection.contains(Name) && Collection.at(Name) != nullptr)
		{
			Bone2* NewChild = Collection.at(Name);

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

	bool AddChild(int index, std::vector<Bone2*> Collection)
	{
		if (index > Collection.size())
		{
			Bone2* NewChild = Collection.at(index);

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

		for (Bone2* Element : Children)
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

	Bone2* m_ParentBone;

	std::vector<Bone2*>		Children;

	int						NumberOfChildren;
	int						MaximumNumberOfChildBones = 25;

	std::string				Name;
};
