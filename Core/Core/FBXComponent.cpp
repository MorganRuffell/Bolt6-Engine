
#include "FBXComponent.h"
#include <Core/Core/Vertex.h>

using namespace fbxsdk;


//#define USE_EXTERNAL_FBX_PLUGINS
#define USE_REDUDANT_LOADING

void FBXComponent::InitalizeComponent()
{
}

bool FBXComponent::TerminateComponent()
{
	return false;
}

//


void FBXComponent::GetAnimatedMatrix(float DeltaTime, Skeleton* _skl)
{
	FbxTime repeat = 0;
	repeat.SetSecondDouble(3.3);
	_skl->Time.SetSecondDouble(ImporterTime);

	ImporterTime += DeltaTime;

	if (_skl->Time > repeat)
	{

		ImporterTime -= 3.3;
		_skl->Time.SetSecondDouble(ImporterTime);
	}
	else if (_skl->Time < 0)
	{
		ImporterTime += 3.3;
		_skl->Time.SetSecondDouble(ImporterTime);
	}

	for (int i = 0; i < _skl->mBones.size(); i++)
	{
		_skl->mBones[i]->SetboneTransform(GetJointGlobalTransform(i, _skl->mBones, _skl));
	}
}

void FBXComponent::InitalizeFBXObjects()
{
	fbxManager = fbxsdk::FbxManager::Create();

	if (!fbxManager)
	{
		std::cout << "FBX Manager Initalization Failed" << std::endl;
		return;
	}

	fbxInputOutputSettings = fbxsdk::FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(fbxInputOutputSettings);

	//Until I know what these are, just lock them away..
#ifdef USE_EXTERNAL_FBX_PLUGINS

	FbxString LoadingPath = FbxGetApplicationDirectory();
	assert(LoadingPath.IsEmpty() != true);
	fbxManager->LoadPluginsDirectory(LoadingPath.Buffer());

#endif // USE_EXTERNAL_FBX_PLUGINS


	assert(fbxInputOutputSettings != nullptr);

	//Remember to target the test scene manually
	scene = FbxScene::Create(fbxManager, "FbxSceneAlpha");
	fbxManager->SetIOSettings(fbxInputOutputSettings);

	if (!scene)
	{
		std::cout << "Unable To Initalize FBX Scene. Terminating..." << std::endl;
	}

	std::cout << "FBX Scene Initalized Correctly" << std::endl;
}

void FBXComponent::TerminateFBXObjects()
{
#ifdef USE_EXTERNAL_FBX_PLUGINS

	fbxManager->UnloadPlugins();
#endif

	fbxManager->GetReferenceCount();
	fbxManager->GetIOSettings()->Destroy();

	if (fbxManager)
	{
		fbxManager->Destroy();
	}
}

void FBXComponent::InitalizeImportSettings()
{
	ImportSettings.ReplaceTriangulatedGeometry = true;
	ImportSettings.ReplaceTriangulatedGeometry = false;
}

void FBXComponent::InitalizeImporters(const char* Filename)
{
	assert(fbxManager != nullptr);

	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;

	bool Result;

	fbxsdk::FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	PrimaryImporter = FbxImporter::Create(fbxManager, Filename);

	const bool lImportStatus = PrimaryImporter->Initialize(Filename, -1, fbxManager->GetIOSettings());

	//Why does this not work! -- It makes no sense.
	PrimaryImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = PrimaryImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (PrimaryImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
		}

		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", Filename, lFileMajor, lFileMinor, lFileRevision);
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
	FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", Filename, lFileMajor, lFileMinor, lFileRevision);
}

bool FBXComponent::LoadFBXScene(const char* Filename, FbxScene* Scene, World* world)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;

	bool Result;

	fbxsdk::FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	InitalizeImporters(Filename);

	std::cout << "FBX file format for this is FBX SDK" << lSDKMajor << "." << lSDKMinor << std::endl;

	auto Importer = PrimaryImporter;

	fbxInputOutputSettings->SetBoolProp(IMP_FBX_MATERIAL, true);
	fbxInputOutputSettings->SetBoolProp(IMP_FBX_TEXTURE, true);
	fbxInputOutputSettings->SetBoolProp(IMP_FBX_LINK, true);
	fbxInputOutputSettings->SetBoolProp(IMP_FBX_SHAPE, true);
	fbxInputOutputSettings->SetBoolProp(IMP_FBX_GOBO, true);
	fbxInputOutputSettings->SetBoolProp(IMP_FBX_ANIMATION, true);
	fbxInputOutputSettings->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);

	Result = Importer->Import(Scene, false);
	Importer->ContentUnload();

	return Result;
}

fbxsdk::FbxNodeAttribute* FBXComponent::DetermineTypeOfNode(fbxsdk::FbxNode* Node)
{
	return Node->GetNodeAttribute();
}


StaticMesh* FBXComponent::CreateStaticMesh(fbxsdk::FbxNode* Node, Accelerator* _accel)
{
	assert(fbxManager != nullptr);
	assert(_accel != nullptr);

	fbxsdk::FbxString Name = Node->GetName();
	fbxsdk::FbxGeometryConverter converter(fbxManager);

	converter.Triangulate(scene, ImportSettings.ReplaceTriangulatedGeometry, ImportSettings.UseLegacyTriangulation);

	if (!DetermineTypeOfNode(Node)->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
	{
		return nullptr;
	}

	std::vector<Vertex1> Vertexes;
	std::vector<UINT> Indicies;

	fbxsdk::FbxMesh* Mesh = (fbxsdk::FbxMesh*)Node->GetNodeAttribute();
	FbxVector4* ControlPoints = Mesh->GetControlPoints();
	UINT VertexCount = Mesh->GetControlPointsCount();

	for (unsigned int i = 0; i < VertexCount; i++)
	{
		Vertex1 v;

		v.Position.x = ControlPoints[i].mData[0];
		v.Position.y = ControlPoints[i].mData[1];
		v.Position.z = ControlPoints[i].mData[2];

		v.Normal = XMFLOAT3(0, 0, 0);
		Vertexes.push_back(v);
	}

	int PolygonCount = Mesh->GetPolygonCount();
	int PolygonSize = Mesh->GetPolygonGroup(0);
	UINT indexCount = PolygonCount * PolygonSize;

	for (int i = 0; i < Mesh->GetPolygonCount(); i++)
	{
		for (int j = 0; j < Mesh->GetPolygonSize(i); j++)
		{
			int Individual_Indicie;

			Individual_Indicie = Mesh->GetPolygonVertex(i, j);
			Indicies.push_back(Individual_Indicie);

			FbxVector4 norm(0, 0, 0, 0);

			Mesh->GetPolygonVertexNormal(i, j, norm);

			Vertexes[Individual_Indicie].Normal.x += (float)norm.mData[0];		// Vertex Normals
			Vertexes[Individual_Indicie].Normal.y += (float)norm.mData[1];
			Vertexes[Individual_Indicie].Normal.z += (float)norm.mData[2];

			FbxVector2 PolygonUVCoordinates(0, 0);

			const char* uvSet = "FILENAME DELETE WHEN LOADING TEXTURES TO MATERIALS";
			bool UVFlag = true;

			Mesh->GetPolygonVertexUV(i, j, uvSet, PolygonUVCoordinates, UVFlag);
		}
	}

	std::string MeshName = Node->GetName();

	StaticMesh* SM = new StaticMesh(&Vertexes[0], VertexCount, &Indicies[0], indexCount, _accel, MeshName);

	//SM->CalculateTangents(&Vertexes[0], VertexCount, &Indicies[0], indexCount);

	return SM;
}

//Static Meshes DO NOT contain any animation data.
StaticMesh* FBXComponent::CreateStaticMesh(fbxsdk::FbxNode* Node, Accelerator* _accel, std::string& MeshName)
{
	assert(fbxManager != nullptr);
	assert(_accel != nullptr);
	assert(Node != nullptr);

	fbxsdk::FbxString Name = Node->GetName();
	fbxsdk::FbxGeometryConverter converter(fbxManager);

	converter.Triangulate(scene, ImportSettings.ReplaceTriangulatedGeometry, ImportSettings.UseLegacyTriangulation);

	if (!DetermineTypeOfNode(Node)->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
	{
		return nullptr;
	}

	std::vector<Vertex1> Vertexes;
	std::vector<UINT> Indicies;

	fbxsdk::FbxMesh* Mesh = (fbxsdk::FbxMesh*)Node->GetNodeAttribute();

	FbxVector4* ControlPoints = Mesh->GetControlPoints();
	UINT VertexCount = Mesh->GetControlPointsCount();

	for (unsigned int i = 0; i < VertexCount; i++)
	{
		Vertex1 v;

		v.Position.x = ControlPoints[i].mData[0];
		v.Position.y = ControlPoints[i].mData[1];
		v.Position.z = ControlPoints[i].mData[2];

		v.Normal = XMFLOAT3(0, 0, 0);
		Vertexes.push_back(v);
	}

	int PolygonCount = Mesh->GetPolygonCount();
	int PolygonSize = Mesh->GetPolygonGroup(0);
	UINT indexCount = PolygonCount * PolygonSize;

	for (int i = 0; i < Mesh->GetPolygonCount(); i++)
	{
		for (int j = 0; j < Mesh->GetPolygonSize(i); j++)
		{
			int Individual_Indicie;

			Individual_Indicie = Mesh->GetPolygonVertex(i, j);
			Indicies.push_back(Individual_Indicie);

			FbxVector4 norm(0, 0, 0, 0);

			Mesh->GetPolygonVertexNormal(i, j, norm);

			Vertexes[Individual_Indicie].Normal.x += (float)norm.mData[0];		// Vertex Normals
			Vertexes[Individual_Indicie].Normal.y += (float)norm.mData[1];
			Vertexes[Individual_Indicie].Normal.z += (float)norm.mData[2];

			FbxVector2 PolygonUVCoordinates(0, 0);

			const char* uvSet = "FILENAME DELETE WHEN LOADING TEXTURES TO MATERIALS";
			bool UVFlag = true;

			Mesh->GetPolygonVertexUV(i, j, uvSet, PolygonUVCoordinates, UVFlag);
		}
	}


	StaticMesh* SM = new StaticMesh(&Vertexes[0], VertexCount, &Indicies[0], indexCount, _accel, MeshName);
	SM->SetVertexCount(VertexCount);
	//SM->CalculateTangents(&Vertexes[0], VertexCount, &Indicies[0], indexCount);

	return SM;
}

//Dynamic Meshes DO Contain animation data, if you try to load one and not the other this will not work
DynamicMesh* FBXComponent::CreateDynamicMesh(fbxsdk::FbxNode* Node, Accelerator* _accel, Skeleton* DynamicMeshSkeleton)
{
	assert(fbxManager != nullptr);
	assert(_accel != nullptr);
	assert(Node != nullptr);


	fbxsdk::FbxString Name = Node->GetName();
	fbxsdk::FbxGeometryConverter converter(fbxManager);

	converter.Triangulate(scene, ImportSettings.ReplaceTriangulatedGeometry, ImportSettings.UseLegacyTriangulation);

	if (!DetermineTypeOfNode(Node)->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
	{
		return nullptr;
	}

	std::vector<Vertex3> Vertexes;
	std::vector<UINT> Indicies;

	fbxsdk::FbxMesh* Mesh = (fbxsdk::FbxMesh*)Node->GetNodeAttribute();

	FbxVector4* ControlPoints = Mesh->GetControlPoints();
	UINT VertexCount = Mesh->GetControlPointsCount();

	for (unsigned int i = 0; i < VertexCount; i++)
	{
		Vertex3 v;

		v.Position.x = ControlPoints[i].mData[0];
		v.Position.y = ControlPoints[i].mData[1];
		v.Position.z = ControlPoints[i].mData[2];

		v.Normal = XMFLOAT3(0, 0, 0);
		Vertexes.push_back(v);
	}

	int PolygonCount = Mesh->GetPolygonCount();
	int PolygonSize = Mesh->GetPolygonGroup(0);
	UINT indexCount = PolygonCount * PolygonSize;

	for (int i = 0; i < Mesh->GetPolygonCount(); i++)
	{
		for (int j = 0; j < Mesh->GetPolygonSize(i); j++)
		{
			int Individual_Indicie;

			Individual_Indicie = Mesh->GetPolygonVertex(i, j);
			Indicies.push_back(Individual_Indicie);

			FbxVector4 norm(0, 0, 0, 0);

			Mesh->GetPolygonVertexNormal(i, j, norm);

			Vertexes[Individual_Indicie].Normal.x += (float)norm.mData[0];		// Vertex Normals
			Vertexes[Individual_Indicie].Normal.y += (float)norm.mData[1];
			Vertexes[Individual_Indicie].Normal.z += (float)norm.mData[2];

			FbxVector2 PolygonUVCoordinates(0, 0);

			const char* uvSet = "FILENAME DELETE WHEN LOADING TEXTURES TO MATERIALS";
			bool UVFlag = true;

			Mesh->GetPolygonVertexUV(i, j, uvSet, PolygonUVCoordinates, UVFlag);
		}
	}

	assert(DynamicMeshSkeleton != nullptr);


	int NumberOfDeformers = Mesh->GetDeformerCount();

	if (NumberOfDeformers == 0) { return new DynamicMesh(DynamicMeshSkeleton, &Vertexes[0], VertexCount, &Indicies[0], indexCount, _accel); }

	for (int deformerIndex = 0; deformerIndex < NumberOfDeformers; deformerIndex++)
	{
		fbxsdk::FbxSkin* skin = reinterpret_cast<FbxSkin*>(Mesh->GetDeformer(0, fbxsdk::FbxDeformer::eSkin));
		int NumberOfSkinClusters = skin->GetClusterCount();

		//Enumerate all of the skin clusters inside of the mesh, skin clusters act as a subset of a geometry control points -- Similar to a geometry shader
		for (int indexOfSkinCluster = 0; indexOfSkinCluster < NumberOfSkinClusters; ++NumberOfSkinClusters)
		{
			//Get the current cluster and it's link mode.
			fbxsdk::FbxCluster* CurrentCluster = skin->GetCluster(indexOfSkinCluster);
			fbxsdk::FbxCluster::ELinkMode JointLinkMode = CurrentCluster->GetLinkMode();

			std::string CurrentJointName = CurrentCluster->GetLink()->GetName();

			int CurrentBoneIndex = FindBoneIndex(CurrentJointName, DynamicMeshSkeleton->mBones);

			//Three matrixes, one for the transform of the cluster (the cluster of control points)
			fbxsdk::FbxAMatrix              transformMatrix;
			fbxsdk::FbxAMatrix              transformLinkMatrix;
			fbxsdk::FbxAMatrix              globalBindposeInverseMatrix;

			CurrentCluster->GetTransformMatrix(transformMatrix);
			CurrentCluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse();

			//A control point is a synoynm for a vertex
			int ControlPointIndCount = CurrentCluster->GetControlPointIndicesCount();

			globalBindposeInverseMatrix = transformLinkMatrix.Inverse();
			transformLinkMatrix = transformLinkMatrix.Transpose();

			//Get the inverse of each bone transform.
			DynamicMeshSkeleton->mBones[CurrentBoneIndex]->InvBoneTransform = XMFLOAT4X4((float)globalBindposeInverseMatrix.GetRow(0)[0],
				(float)globalBindposeInverseMatrix.GetRow(0)[1], (float)globalBindposeInverseMatrix.GetRow(0)[2],
				(float)globalBindposeInverseMatrix.GetRow(0)[3], (float)globalBindposeInverseMatrix.GetRow(1)[0],
				(float)globalBindposeInverseMatrix.GetRow(1)[1], (float)globalBindposeInverseMatrix.GetRow(1)[2],
				(float)globalBindposeInverseMatrix.GetRow(1)[3], (float)globalBindposeInverseMatrix.GetRow(2)[0],
				(float)globalBindposeInverseMatrix.GetRow(2)[1], (float)globalBindposeInverseMatrix.GetRow(2)[2],
				(float)globalBindposeInverseMatrix.GetRow(2)[3],
				(float)globalBindposeInverseMatrix.GetRow(3)[0], (float)globalBindposeInverseMatrix.GetRow(3)[1],
				(float)globalBindposeInverseMatrix.GetRow(3)[2], (float)globalBindposeInverseMatrix.GetRow(3)[3]);

			//Set the FBX Node on the Bone itself
			DynamicMeshSkeleton->mBones[CurrentBoneIndex]->SetFbxNode(CurrentCluster->GetLink());

			//Set the transform of the Bone as an XMFloat4x4 Matrix
			DynamicMeshSkeleton->mBones[CurrentBoneIndex]->BoneTransform = XMFLOAT4X4((float)transformLinkMatrix.GetRow(0)[0],
				(float)transformLinkMatrix.GetRow(0)[1], (float)transformLinkMatrix.GetRow(0)[2],
				(float)transformLinkMatrix.GetRow(0)[3], (float)transformLinkMatrix.GetRow(1)[0],
				(float)transformLinkMatrix.GetRow(1)[1], (float)transformLinkMatrix.GetRow(1)[2],
				(float)transformLinkMatrix.GetRow(1)[3], (float)transformLinkMatrix.GetRow(2)[0],
				(float)transformLinkMatrix.GetRow(2)[1], (float)transformLinkMatrix.GetRow(2)[2],
				(float)transformLinkMatrix.GetRow(2)[3], (float)transformLinkMatrix.GetRow(3)[0],
				(float)transformLinkMatrix.GetRow(3)[1], (float)transformLinkMatrix.GetRow(3)[2],
				(float)transformLinkMatrix.GetRow(3)[3]); //

			DynamicMeshSkeleton->mBones[CurrentBoneIndex]->SetFbxNode(CurrentCluster->GetLink());
			DynamicMeshSkeleton->mBones[CurrentBoneIndex]->SetFbxTransform(&transformLinkMatrix);
			DynamicMeshSkeleton->mBones[CurrentBoneIndex]->SetBoneIndex(CurrentBoneIndex);

			//For all of the control points inside each cluster, set thier weighting by getting the indicies.
			for (int i = 0; i < CurrentCluster->GetControlPointIndicesCount(); ++i)
			{
				//indexes and Ids of each vertex
				int index = CurrentCluster->GetControlPointIndices()[i];
				int vertexid = Indicies[CurrentCluster->GetControlPointIndices()[i]];

				if (Vertexes[index].Boneids.x == -1 && Vertexes[index].Weights.x == -1)
				{
					Vertexes[index].Boneids.x = (float)CurrentBoneIndex;
					Vertexes[index].Weights.x = (float)CurrentCluster->GetControlPointWeights()[i];
				}
				else if (Vertexes[index].Boneids.y == -1 && Vertexes[index].Weights.y == -1)
				{
					Vertexes[index].Boneids.y = (float)CurrentBoneIndex;
					Vertexes[index].Weights.y = (float)CurrentCluster->GetControlPointWeights()[i];
				}
				else if (Vertexes[index].Boneids.z == -1 && Vertexes[index].Weights.z == -1)
				{
					Vertexes[index].Boneids.z = (float)CurrentBoneIndex;
					Vertexes[index].Weights.z = (float)CurrentCluster->GetControlPointWeights()[i];
				}

				else
				{
					float currentWeight = (float)CurrentCluster->GetControlPointWeights()[i];

					if (Vertexes[index].Weights.x < Vertexes[index].Weights.y)
					{
						if (Vertexes[index].Weights.x < Vertexes[index].Weights.z)
						{
							if (Vertexes[index].Weights.x < Vertexes[index].Weights.w)
							{
								Vertexes[index].Boneids.x = (float)CurrentBoneIndex;
								Vertexes[index].Weights.x = (float)currentWeight;
							}
							else
							{
								Vertexes[index].Boneids.w = (float)CurrentBoneIndex;
								Vertexes[index].Weights.w = (float)currentWeight;
							}
						}
						else if (Vertexes[index].Weights.w < Vertexes[index].Weights.z)
						{
							Vertexes[index].Boneids.w = (float)CurrentBoneIndex;
							Vertexes[index].Weights.w = (float)currentWeight;
						}
						else
						{
							Vertexes[index].Boneids.z = (float)CurrentBoneIndex;
							Vertexes[index].Weights.z = (float)currentWeight;
						}
					}

				}


			}

		}

		// https://download.autodesk.com/us/fbx/20112/fbx_sdk_help/index.html?url=WS1a9193826455f5ff1f92379812724681e696651.htm,topicNumber=d0e7429 -- Look here

		//FbxSkin* skin = reinterpret_cast<FbxSkin*>(Mesh->GetDeformer(0, fbxsdk::FbxDeformer::eBlendShape));
	}

	DynamicMesh* DM = new DynamicMesh(DynamicMeshSkeleton, &Vertexes[0], VertexCount, &Indicies[0], indexCount, _accel);
	//DM->CalculateTangents(&Vertexes[0], VertexCount, &Indicies[0], indexCount);

	return DM;
}

int FBXComponent::FindBoneIndex(const std::string& name, std::vector<Bone2*>& BoneCollection)
{
	for (int i = 0; i < BoneCollection.size(); i++)
	{
		if (BoneCollection[i]->GetName() == name)
		{
			return i;
		}
	}
	return 0;
}

XMFLOAT4X4 FBXComponent::GetJointGlobalTransform(int val, std::vector<Bone2*> Bones, Skeleton* _skl)
{
	FbxAMatrix jointTransform;
	FbxNode* boneNode = Bones[val]->GetFbxNode();

	if (boneNode)
	{
		jointTransform = boneNode->EvaluateGlobalTransform(_skl->Time);
	}


	XMFLOAT4X4 globalTransform = FbxAMatrixToXMFloat4x4(jointTransform);
	return XMFLOAT4X4(globalTransform);
}


void FBXComponent::GetMatrixesFromMesh(fbxsdk::FbxNode* Node, Accelerator* _accel, std::vector<Bone2>&)
{
}


XMFLOAT4X4 FBXComponent::FbxAMatrixToXMFloat4x4(FbxAMatrix Matrix)
{
	Matrix = Matrix.Transpose();
	return XMFLOAT4X4((float)Matrix.GetRow(0)[0],
		(float)Matrix.GetRow(0)[1], (float)Matrix.GetRow(0)[2],
		(float)Matrix.GetRow(0)[3], (float)Matrix.GetRow(1)[0],
		(float)Matrix.GetRow(1)[1], (float)Matrix.GetRow(1)[2],
		(float)Matrix.GetRow(1)[3], (float)Matrix.GetRow(2)[0],
		(float)Matrix.GetRow(2)[1], (float)Matrix.GetRow(2)[2],
		(float)Matrix.GetRow(2)[3], (float)Matrix.GetRow(3)[0],
		(float)Matrix.GetRow(3)[1], (float)Matrix.GetRow(3)[2],
		(float)Matrix.GetRow(3)[3]);
}
