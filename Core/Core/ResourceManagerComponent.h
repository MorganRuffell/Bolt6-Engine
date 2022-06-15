// Morgan Ruffell, 2022
// Generic Engine Resource Manager, C++20

// Autodesk FBX SDK - Autodesk Incorporated 2022


#pragma once

/*
*   Generic Engine Resource Manager
*   Designed to be added as a unique/raw pointer to an engine.
* 
*   Quick, and relatively painless. Only works with Autodesk Filmbox files,
*   Will expand to work with .objs and any other requests later on.
* 
*   Woo.
*/

#include "EngineComponent.h"
#include <Core/Core/FBXComponent.h>
#include <iostream>

namespace InitData
{
    enum MeshTypeToInitalize
    {
        Static,
        Dynamic
    };

    enum PolygonDensity
    {
        Ultra,          //Zbrush sculpts, like the ones you see in nanite in UE5
        High,
        Medium,
        Low             //Something like my first sword in blender from like 2017
    };

}


struct MeshData
{
    LPCWSTR Filelocation;
    LPCWSTR DiffuseTexture;
    LPCWSTR NormalTexture;
    std::string MeshName;

    InitData::MeshTypeToInitalize MeshType;
    InitData::PolygonDensity      PolygonDensity;
};


class ResourceManagerComponent : public EngineComponent
{

    ResourceManagerComponent() = delete;
   
public:

    ResourceManagerComponent(Accelerator* _accel, World* world)
    {
        assert(world != nullptr);

        if (_accel)
        {
            m_Accelerator = _accel;
        }
        else
        {
            std::cout << "The Accelerator cannot be null" << std::endl;
        }

        m_World = world;

        InitalizeComponent();

    }

    static ResourceManagerComponent* Instance;

protected:

    FBXComponent*       m_FBXImportComponent;
    Accelerator*        m_Accelerator;
    World*              m_World;

protected:

    int ResourceCountLoaded = 0;

    //I Love making big things... But this is about delivering something quickly.
    virtual void LoadDynamicMeshResource(MeshData MeshData);
    virtual void LoadStaticMeshResource(MeshData MeshData);
    virtual void LoadTestResources();

    
protected:

    static ResourceManagerComponent* GetInstance()
    {
        return Instance;
    }

    // Inherited via EngineComponent
    virtual void InitalizeComponent();

    virtual bool TerminateComponent();

};

