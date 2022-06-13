#include "FBXComponent.h"

using namespace fbxsdk;

void FBXComponent::InitalizeComponent()
{
}

bool FBXComponent::TerminateComponent()
{
    return false;
}

//
FbxImporter* FBXComponent::GetImporter()
{
    auto SuccessfulImporter = fbxsdk::FbxStatus(fbxsdk::FbxStatus::eSuccess);
    return Importers.at(SuccessfulImporter);
}

void FBXComponent::InitalizeFBXObjects()
{
    fbxManager = FbxManager::Create();

    if (!fbxManager)
    {
        std::cout << "FBX Manager Initalization Failed" << std::endl;
        return;
    }
    
    fbxInputOutputSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
    fbxManager->SetIOSettings(fbxInputOutputSettings);

    //Until I know what these are, just lock them away..
#ifdef USE_EXTERNAL_FBX_PLUGINS

    FbxString LoadingPath = FbxGetApplicationDirectory();
    assert(LoadingPath.IsEmpty() != true);
    fbxManager->LoadPluginsDirectory(LoadingPath.Buffer());

#endif // USE_EXTERNAL_FBX_PLUGINS

    if (scene != nullptr)
    {
        std::cout << "You cannot initalize these components, there is already a scene loaded" << std::endl;
        std::cout << "Only one scene is supported right now" << std::endl;
        return;
    }
    else
    {
        assert(fbxInputOutputSettings != nullptr);

        scene = FbxScene::Create(fbxManager, "FbxSceneAlpha");
        fbxManager->SetIOSettings(fbxInputOutputSettings);

        if (!scene)
        {
            std::cout << "Unable To Initalize FBX Scene. Terminating..." << std::endl;
        }
        
    }
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

void FBXComponent::GetFBXScene(FbxString name, FbxString Filepath)
{



}

void FBXComponent::InitalizeImporters(const char* Filename)
{
    assert(fbxManager != nullptr);
    
    FbxImporter* PrimaryImporter = FbxImporter::Create(fbxManager, Filename);
    FbxImporter* SecondaryImporter = FbxImporter::Create(fbxManager, Filename);
    FbxImporter* TertiaryImporter = FbxImporter::Create(fbxManager, Filename);

    PrimaryImporter->Initialize(Filename, -1, fbxManager->GetIOSettings());
    SecondaryImporter->Initialize(Filename, -1, fbxManager->GetIOSettings());
    TertiaryImporter->Initialize(Filename, -1, fbxManager->GetIOSettings());

    fbxsdk::FbxStatus PStatus = PrimaryImporter->GetStatus();
    fbxsdk::FbxStatus SStatus = SecondaryImporter->GetStatus();
    fbxsdk::FbxStatus TStatus = TertiaryImporter->GetStatus();

    if (PStatus == fbxsdk::FbxStatus::EStatusCode::eSuccess)
    {
        std::cout << "Fbx Primary Import Status is Successful" << std::endl;
    }

    if (SStatus == fbxsdk::FbxStatus::EStatusCode::eSuccess)
    {
        std::cout << "Fbx Primary Import Status is Successful" << std::endl;
    }

    if (TStatus == fbxsdk::FbxStatus::EStatusCode::eSuccess)
    {
        std::cout << "Fbx Primary Import Status is Successful" << std::endl;
    }

    Importers.insert({ PStatus, PrimaryImporter });
    Importers.insert({ SStatus, SecondaryImporter });
    Importers.insert({ TStatus, TertiaryImporter });
}




bool FBXComponent::LoadFBXScene(const char* Filename, FbxScene* Scene, World* world)
{
    int lFileMajor, lFileMinor, lFileRevision;
    int lSDKMajor, lSDKMinor, lSDKRevision;

    bool Result;

    FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

    InitalizeImporters(Filename);

    std::cout << "FBX file format for this is FBX SDK" << lSDKMajor << "." << lSDKMinor << std::endl;

    auto Importer = GetImporter();

    if (Importer->IsFBX())
    {
        fbxInputOutputSettings->SetBoolProp(IMP_FBX_MATERIAL, true);
        fbxInputOutputSettings->SetBoolProp(IMP_FBX_TEXTURE, true);
        fbxInputOutputSettings->SetBoolProp(IMP_FBX_LINK, true);
        fbxInputOutputSettings->SetBoolProp(IMP_FBX_SHAPE, true);
        fbxInputOutputSettings->SetBoolProp(IMP_FBX_GOBO, true);
        fbxInputOutputSettings->SetBoolProp(IMP_FBX_ANIMATION, true);
        fbxInputOutputSettings->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }

    Result = Importer->Import(Scene);
    Importer->ContentUnload();

    return Result;
}

FbxNodeAttribute::EType FBXComponent::DetermineTypeOfNode(fbxsdk::FbxNode* Node)
{
    return Node->GetNodeAttribute()->GetAttributeType();
}

//Recursive method that gets all of the bones and loads them into a skeleton on a dynamic mesh
void FBXComponent::LoadSkeletonJoints(fbxsdk::FbxNode* Node, Skeleton* s_kl)
{
    if (DetermineTypeOfNode(Node) == FbxNodeAttribute::EType::eSkeleton)
    {
        if (s_kl == nullptr)
        {
            fbxsdk::FbxAMatrix BonePos;
            fbxsdk::FbxAMatrix GlobalMatrix;

            fbxsdk::FbxString Name = Node->GetName();
            fbxsdk::FbxVector4 Translation = Node->GetGeometricTranslation(FbxNode::eSourcePivot);
            fbxsdk::FbxVector4 Rotation = Node->GetGeometricRotation(FbxNode::eSourcePivot);
            fbxsdk::FbxVector4 Scale = Node->GetGeometricScaling(FbxNode::eSourcePivot);

            GlobalMatrix = Node->EvaluateGlobalTransform();

            BonePos.SetT(Translation);
            BonePos.SetR(Rotation);
            BonePos.SetS(Scale);

            fbxsdk::FbxAMatrix FinalMatrix = GlobalMatrix * BonePos;

            const char* stdName = Name;

            Bone2* Bone = new Bone2(stdName, FbxAMatrixToXMFloat4x4(FinalMatrix), Parent);
            s_kl = new Skeleton(Bone);
            s_kl->mBones.push_back(Bone);
        }
    }

    int childCount = Node->GetChildCount();

    for (int i = 0; i < childCount; i++)
    {
        LoadSkeletonJoints(Node->GetChild(i), s_kl);
    }
}

StaticMesh* FBXComponent::GetStaticMesh(::FbxNode* Node, Accelerator* _accel)
{




    return nullptr;
}

DynamicMesh* FBXComponent::GetDynamicMesh(::FbxNode* Node, Accelerator* _accel)
{
    return nullptr;
}

Skeleton* FBXComponent::GetSkeleton(::FbxNode* Node, Accelerator* _accel)
{
    return nullptr;
}

void FBXComponent::GetMatrixesFromMesh(::FbxNode* Node, Accelerator* _accel, std::vector<Socket>&)
{
}

XMFLOAT4X4 FBXComponent::GetJointGlobalTransform(int, std::vector<Socket>& Collection)
{
    return XMFLOAT4X4();
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
