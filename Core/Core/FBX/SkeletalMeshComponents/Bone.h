#pragma once

#include <DirectXMath.h>
#include <fbxsdk.h>
#include <string>
#include <map>
#include <vector>

using namespace DirectX;
using namespace fbxsdk;

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

	//Default constructor for renderer use only.
	Bone2()
	{

	}

public:

	//Constructor for creating a new bone relative to it's parent
	Bone2(std::string& Name, XMFLOAT4X4 OffsetPosition, Bone2* Parent) :
		mFBXNode(nullptr)
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
	Bone2(XMFLOAT4X4 OffsetPosition, Bone2* Parent) :
		mFBXNode(nullptr)
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
			SetBoneIndex(0);

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

	//This kills all of the children :(
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


	bool SetFbxNode(FbxNode* Node)
	{
		assert(Node != nullptr);

		if (mFBXNode == nullptr)
		{
			mFBXNode = Node;
			return true;
		}
		else
		{
			//Not sure about my popinter syntaxx here, might have to dereference it first
			mFBXNode = Node;
			return true;
		}

		return false;

	}

	FbxNode* GetFbxNode()
	{
		return mFBXNode;
	}

public:

	FbxAMatrix* GetFbxMatrix()
	{
		return mFbxTransform;
	}

	bool SetFbxTransform(FbxAMatrix* Transform)
	{
		assert(Transform != nullptr);

		if (mFbxTransform == nullptr)
		{
			mFbxTransform = Transform;
			return true;
		}
		else
		{
			//Not sure about my popinter syntaxx here, might have to dereference it first
			mFbxTransform = Transform;
			return true;
		}

		return false;

	}


public:

	int GetBoneIndex()
	{
		return m_BoneIndex;
	}

	void SetBoneIndex(int index)
	{
		m_BoneIndex = index;
	}

public:

	XMFLOAT4X4 GetBoneTransform()
	{
		return BoneTransform;
	}

	void SetboneTransform(XMFLOAT4X4 Trans)
	{
		BoneTransform = Trans;
	}

public:

	DirectX::XMFLOAT4X4		BoneTransform = {};
	DirectX::XMFLOAT4X4		InvBoneTransform = {};

	//We need both the inverse bindpose and relative bindpose
	DirectX::XMFLOAT4X4		mRelativeBindposeInverse = {};
	DirectX::XMFLOAT4X4		mRelativeBindpose = {};

protected:

	//Maybe make a bespoke pointer for this if you fancy it :D
	FbxNode*				mFBXNode;
	FbxAMatrix*				mFbxTransform = {};

protected:

	BoneTranslationMode		TranslationMode;
	BoneType				TypeOfBone;

protected:
	
	//The index of the bone is represented
	int						m_BoneIndex;

protected:

	Bone2* m_ParentBone;

	std::vector<Bone2*>		Children;

	int						NumberOfChildren;
	int						MaximumNumberOfChildBones = 25;

	std::string				Name;
};
