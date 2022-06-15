#include "ResourceManagerComponent.h"


void ResourceManagerComponent::InitalizeComponent()
{
    m_FBXImportComponent = new FBXComponent();

    std::cout << "FBX Import Component Initalized Successfully" << std::endl;

    MeshData Animation = {};
    Animation.Filelocation = "";
    Animation.DiffuseTexture = "";
    Animation.NormalTexture = "";

    MeshData Static = {};
    Animation.Filelocation = "";
    Animation.DiffuseTexture = "";
    Animation.NormalTexture = "";

    LoadTestResources();
    LoadDynamicMeshResource(Animation);
    LoadStaticMeshResource(Static);


    std::cout << "Resource Manager Component Initalized Successfully" << std::endl;
}

void ResourceManagerComponent::LoadDynamicMeshResource(MeshData MeshData)
{
    assert(m_FBXImportComponent != nullptr);
    assert(m_Accelerator != nullptr);

    m_FBXImportComponent->LoadFBXScene(MeshData.Filelocation, m_FBXImportComponent->GetMostRecentScene(), m_World);

    DynamicMesh* Anim = m_FBXImportComponent->CreateDynamicMesh(m_FBXImportComponent->GetMostRecentScene()->GetRootNode(), m_Accelerator);
    Anim->SetName(MeshData.MeshName);

    TextureContext DiffuseContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 2048;
    DiffuseContext.Resolution.DimensionY = 2048;
    DiffuseContext.Type = Diffuse;
    DiffuseContext.TextureFilename = (LPCWSTR) MeshData.DiffuseTexture;

    TextureContext NormalContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 2048;
    DiffuseContext.Resolution.DimensionY = 2048;
    DiffuseContext.Type = Normal;
    DiffuseContext.TextureFilename = (LPCWSTR) MeshData.NormalTexture;

    Anim->CreateMaterial(m_Accelerator, MeshData.MeshName, DiffuseContext, NormalContext);
}

void ResourceManagerComponent::LoadStaticMeshResource(MeshData MeshData)
{
    assert(m_FBXImportComponent != nullptr);
    assert(m_Accelerator != nullptr);

    m_FBXImportComponent->LoadFBXScene(MeshData.Filelocation, m_FBXImportComponent->GetMostRecentScene(), m_World);

    StaticMesh* SM = m_FBXImportComponent->CreateStaticMesh(m_FBXImportComponent->GetMostRecentScene()->GetRootNode(), m_Accelerator, MeshData.MeshName);

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

    SM->CreateMaterial(m_Accelerator, MeshData.MeshName, DiffuseContext, NormalContext);
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

    //The texture context acts similar to a descriptor struct in OpenGL, we indicate options and it creates it!
    TextureContext DiffuseContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 2048;
    DiffuseContext.Resolution.DimensionY = 2048;
    DiffuseContext.Type = Diffuse;
    DiffuseContext.TextureFilename = L"/LocationOFTexture";

    TextureContext NormalContext = {};
    DiffuseContext.AllowOverwriting = true;
    DiffuseContext.Resolution.DimensionX = 2048;
    DiffuseContext.Resolution.DimensionY = 2048;
    DiffuseContext.Type = Normal;
    DiffuseContext.TextureFilename = L"/LocationOFTexture";

    Stage->CreateMaterial(m_Accelerator, SMeshName, DiffuseContext, NormalContext);

}


bool ResourceManagerComponent::TerminateComponent()
{
    return false;
}
