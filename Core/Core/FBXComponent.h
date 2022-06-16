#pragma once

/*
	Morgan Ruffell, 2022
	Autodesk (c) FBX are property of Autodesk Incorporated 2022
*/

#include "ImportComponent.h"
#include <fbxsdk.h>
#include <array>
#include "Engine/Graphics/Accelerator.h"

#include "Core/Core/Engine/Animation/AnimationStructures.h"

#include "Core/Core/Mesh/DynamicMesh.h"
#include "Core/Core/Mesh/StaticMesh.h"

#include "Core/Core/FBX/SkeletalMeshComponents/Bone.h"
#include "Core/Core/FBX/SkeletalMeshComponents/Skeleton.h"
#include "Core/Core/FBX/SkeletalMeshComponents/Constraint.h"

#include <Core/Core/World.h>



/*
	FBX Component acts as an intermediary between 
	between the fbx sdk and the rest of the engine 

	Ensure that you have the fbx SDK installed correctly, I usually
	dump all .dlls I'm using in system32
*/

using namespace fbxsdk;

struct FBXImportSettings
{
	/*
	* FBXs triangulation algorithim so that the quads in maya
	* can be rendered has a legacy version that does not support
	* Quads/tris with reversed normals
	*/
	bool UseLegacyTriangulation = false;

	/*
	* Replace older geometryu
	*/
	bool ReplaceTriangulatedGeometry = true;

};


class FBXComponent : public ImportComponent
{
public:
	FBXComponent()
	{
		SetTag(EngineObjTag::Engine);

		InitalizeImportSettings();

		InitalizeFBXObjects();
	}

	~FBXComponent()
	{

	}

public:

	void InitalizeImporters(const char* Filename);

public:

	fbxsdk::FbxManager* fbxManager;
	fbxsdk::FbxIOSettings* fbxInputOutputSettings;

protected:

	void InitalizeFBXObjects();
	void TerminateFBXObjects();

	//void GetFBXScene(fbxsdk::FbxString name, fbxsdk::FbxString Filepath);

public:
	void InitalizeImportSettings();

	fbxsdk::FbxNodeAttribute* DetermineTypeOfNode(fbxsdk::FbxNode* Node);

public:

	bool LoadFBXScene(_In_ const char* Filename, fbxsdk::FbxScene* Scene, World* world);
	void LoadSkeletonJoints(_In_ fbxsdk::FbxNode*, _Inout_ Skeleton* s_kl);
	StaticMesh* CreateStaticMesh(_Inout_ fbxsdk::FbxNode* Node, _In_  Accelerator* _accel);
	StaticMesh* CreateStaticMesh(_Inout_ fbxsdk::FbxNode* Node, _In_  Accelerator* _accel, std::string& MeshName);


public:

	//These two function similar to Unreal, Dynamic Meshes have a skeleton.
	DynamicMesh* CreateDynamicMesh(_Inout_ ::FbxNode* Node, _In_ Accelerator* _accel);

protected:

	int FindBoneIndex(const std::string& name, std::vector<Bone2*>& BoneCollection);

	XMFLOAT4X4 GetJointGlobalTransform(int val, std::vector<Bone2*> Bones, Skeleton* _Skl);
protected:

	void GetMatrixesFromMesh(_Inout_ fbxsdk::FbxNode* Node, _In_ Accelerator* _accel, _In_ std::vector<Bone2>&);

	XMFLOAT4X4 FbxAMatrixToXMFloat4x4(fbxsdk::FbxAMatrix Matrix);

	FBXImportSettings ImportSettings;

	void GetAnimatedMatrix(float DeltaTime, Skeleton* _skl);

public:

	FbxScene* GetMostRecentScene()
	{
		return scene;
	}


protected:
	double ImporterTime;
	FbxScene* scene;
	std::map<fbxsdk::FbxStatus,FbxImporter*> Importers;

	FbxImporter* PrimaryImporter;

protected:

	int ImporterCount = 3;

	bool Result;

	virtual void InitalizeComponent();
	virtual bool TerminateComponent();
};



