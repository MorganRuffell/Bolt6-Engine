#include "ResourceManagerComponent.h"


void ResourceManagerComponent::InitalizeComponent()
{
    m_FBXImportComponent = std::make_unique<FBXComponent>();

    std::cout << "FBX Import Component Initalized Successfully" << std::endl;

    MeshData Animation = {};
    Animation.Filelocation = "\\Scene\\RandoWalk.fbx";
    Animation.DiffuseTexture = L"\Scene\StageDiffuse.png";
    Animation.NormalTexture = L"\Scene\StageNormal.png";

    MeshData Static = {};
    Static.Filelocation = "\\Resources\\Teapot.fbx";
    Static.DiffuseTexture = L"\Scene\\StageDiffuse.png";
    Static.NormalTexture = L"\Scene\\StageNormal.png";
    Static.MeshName = "3DStudioTeapot";

    MeshData Static1 = {};
    Static1.Filelocation = "\\Scene\\Lemonade.fbx";
    Static1.DiffuseTexture = L"\Scene\\StageDiffuse.png";
    Static1.NormalTexture = L"\Scene\\StageNormal.png";
    Static1.MeshName = "LemonadeTest";

    MeshData Static2 = {};
    Static2.Filelocation = "\\Scene\\LemonadeASCII.fbx";
    Static2.DiffuseTexture = L"\Scene\\StageDiffuse.png";
    Static2.NormalTexture = L"\Scene\\StageNormal.png";
    Static2.MeshName = "LemonadeTestASCII";


    LoadTestResources();
    LoadStaticMeshResource(Static);
    LoadStaticMeshResource(Static1);
    LoadStaticMeshResource(Static2);

    //LoadDynamicMeshResource(Animation);


    std::cout << "Resource Manager Component Initalized Successfully" << std::endl;
}

void ResourceManagerComponent::LoadDynamicMeshResource(MeshData MeshData)
{
    assert(m_FBXImportComponent != nullptr);
    assert(m_Accelerator != nullptr);

    m_FBXImportComponent->LoadFBXScene(MeshData.Filelocation, m_FBXImportComponent->GetMostRecentScene(), m_World);

    //This is deliberately a nullptr, it's loaded in through the load skeleton joints.
    Skeleton* DynamicMeshSkeleton = new Skeleton();
    LoadSkeletonJoints(m_FBXImportComponent->GetMostRecentScene()->GetRootNode(), DynamicMeshSkeleton);

    DynamicMesh* Anim = m_FBXImportComponent->CreateDynamicMesh(m_FBXImportComponent->GetMostRecentScene()->GetRootNode()->GetChild(0), m_Accelerator,DynamicMeshSkeleton);
    Anim->SetName(MeshData.MeshName);

    TextureContext DiffuseContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 512;
    DiffuseContext.Resolution.DimensionY = 512;
    DiffuseContext.Type = Diffuse;
    DiffuseContext.TextureFilename = (LPCWSTR) MeshData.DiffuseTexture;

    TextureContext NormalContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 512;
    DiffuseContext.Resolution.DimensionY = 512;
    DiffuseContext.Type = Normal;
    DiffuseContext.TextureFilename = (LPCWSTR) MeshData.NormalTexture;

    Anim->CreateMaterial(m_Accelerator, MeshData.MeshName, DiffuseContext, NormalContext);
 
    m_World->AddDynamicMesh(MeshData.MeshName, Anim);
}

void ResourceManagerComponent::LoadStaticMeshResource(MeshData MeshData)
{
    assert(m_FBXImportComponent != nullptr);
    assert(m_Accelerator != nullptr);

    m_FBXImportComponent->LoadFBXScene(MeshData.Filelocation, m_FBXImportComponent->GetMostRecentScene(), m_World);
    LoadStaticNodes(m_FBXImportComponent->GetMostRecentScene()->GetRootNode());

    StaticMesh* SM = m_FBXImportComponent->CreateStaticMesh(m_FBXImportComponent->GetMostRecentScene()->GetRootNode()->GetChild(0), m_Accelerator, MeshData.MeshName);

    TextureContext DiffuseContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 2048;
    DiffuseContext.Resolution.DimensionY = 2048;
    DiffuseContext.Type = Diffuse;
    DiffuseContext.TextureFilename = (LPCWSTR)MeshData.DiffuseTexture;

    TextureContext NormalContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 2048;
    DiffuseContext.Resolution.DimensionY = 2048;
    DiffuseContext.Type = Normal;
    DiffuseContext.TextureFilename = (LPCWSTR)MeshData.NormalTexture;

    //Fix the vertex shader issue, if we don't solve that we're going nowhere.
    SM->CreateMaterial(m_Accelerator, MeshData.MeshName, DiffuseContext, NormalContext);
  
    m_World->AddStaticMesh(MeshData.MeshName, SM);
}


void ResourceManagerComponent::LoadTestResources()
{
    assert(m_Accelerator != nullptr);

    std::vector<Vertex1> planeVertices = {
        { XMFLOAT3(-0.5f,0,0.5f), XMFLOAT3(0,1,0), XMFLOAT2(0,0) },
        { XMFLOAT3(0.5f,0,0.5f), XMFLOAT3(0,1,0), XMFLOAT2(1,0) },
        { XMFLOAT3(0.5f,0,-0.5f), XMFLOAT3(0,1,0), XMFLOAT2(1,1) },
        { XMFLOAT3(-0.5f,0,-0.5f), XMFLOAT3(0,1,0), XMFLOAT2(0,1)}
    };

    //We form our triangles through the indicies, recall that everything
    //in computer graphics is a triangle. This is just the stage so this is really simple.
    UINT indices[] = { 0,1,2,0,2,3 };

    std::string SMeshName = "Stage Static Mesh";
    auto Stage = new StaticMesh(planeVertices.data(), 4, indices, 6, m_Accelerator, SMeshName);
    Stage->SetVertexCount(4);
    m_World->AddStaticMesh(SMeshName, Stage);

    //The texture context acts similar to a descriptor struct in OpenGL, we indicate options and it creates it!
    TextureContext DiffuseContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 2048;
    DiffuseContext.Resolution.DimensionY = 2048;
    DiffuseContext.Type = Diffuse;
    DiffuseContext.TextureFilename = L"\Scene\StageDiffuse.png";

    TextureContext NormalContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 2048;
    DiffuseContext.Resolution.DimensionY = 2048;
    DiffuseContext.Type = Normal;
    DiffuseContext.TextureFilename = L"..\Bolt6ProgrammingTest\Scene\StageNormal.png";

    Stage->CreateMaterial(m_Accelerator, SMeshName, DiffuseContext);
}


//Recursive method that gets all of the bones and loads them into a skeleton on a dynamic mesh
void ResourceManagerComponent::LoadSkeletonJoints(fbxsdk::FbxNode* Node, Skeleton* s_kl)
{
    if (Node->GetNodeAttribute() && Node->GetNodeAttribute()->GetAttributeType() && Node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
    {

        fbxsdk::FbxAMatrix BonePos;
        fbxsdk::FbxAMatrix GlobalMatrix;

        fbxsdk::FbxString Name = Node->GetName();
        fbxsdk::FbxVector4 Translation = Node->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
        fbxsdk::FbxVector4 Rotation = Node->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
        fbxsdk::FbxVector4 Scale = Node->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

        GlobalMatrix = Node->EvaluateGlobalTransform();

        BonePos.SetT(Translation);
        BonePos.SetR(Rotation);
        BonePos.SetS(Scale);

        fbxsdk::FbxAMatrix FinalMatrix = GlobalMatrix * BonePos;

        const char* stdName = Name;

        
        Bone2* Bone = new Bone2(stdName, m_FBXImportComponent->FbxAMatrixToXMFloat4x4(FinalMatrix), Parent);
        s_kl = new Skeleton(Bone);
        s_kl->mBones.push_back(Bone);
    }

    int childCount = Node->GetChildCount();

    for (int i = 0; i < childCount; i++)
    {
        LoadSkeletonJoints(Node->GetChild(i), s_kl);
    }
}

void ResourceManagerComponent::LoadStaticNodes(FbxNode* node)
{
    FbxString nodeName = node->GetName();
    int childCount = node->GetChildCount();

    for (int i = 0; i < childCount; i++)
    {
        LoadStaticNodes(node->GetChild(i));
    }
}


bool ResourceManagerComponent::TerminateComponent()
{
    return false;
}
