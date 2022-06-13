#pragma once

/*
	Morgan Ruffell, 2022
	Autodesk (c) FBX are property of Autodesk Incorporated 2022
*/

#include "ImportComponent.h"
#include <fbxsdk.h>

#include "Core/Core/Engine/Animation/AnimationStructures.h"

#include "Core/Core/Mesh/DynamicMesh.h"
#include "Core/Core/Mesh/StaticMesh.h"

#include "Core/Core/FBX/SkeletalMeshComponents/Bone.h"
#include "Core/Core/FBX/SkeletalMeshComponents/Skeleton.h"
#include "Core/Core/FBX/SkeletalMeshComponents/Joint.h"

class FBXComponent : public ImportComponent
{
public:
	FBXComponent()
	{
		SetTag(EngineObjTag::Engine);

	}

	~FBXComponent()
	{

	}

public:





protected:

	void InitalizeFBXObjects();
	void TerminateFBXObjects();

protected:

	bool LoadFBXScene(const char*, FbxScene* Scene, World* world);

	void LoadNodes(::FbxNode*, std::vector<Socket*>& Socket);

	StaticMesh* GetStaticMesh(::FbxNode* Node, Accelerator* _accel);
	DynamicMesh* GetDynamicMesh(::FbxNode* Node, Accelerator* _accel);
	Skeleton* GetSkeleton(::FbxNode* Node, Accelerator* _accel);

	void GetMatrixesFromMesh(::FbxNode* Node, Accelerator* _accel, std::vector<Socket>&);


protected:


	bool Result;

	virtual void InitalizeComponent();
	virtual bool TerminateComponent();
};

