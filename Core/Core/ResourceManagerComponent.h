// Morgan Ruffell, 2022
// Resource Manager, C++20


#pragma once

/*
*   Generic FBX resource manager
* 
* 
* 
*/


#include "EngineComponent.h"
#include <Core/Core/FBXComponent.h>
#include <iostream>

/*
    By far the most important class in my solution, 
    this class manages the FBX component, and has a reference to the world component

    It also creates everything in memory ready for the rendering component!
*/

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
    const char* Filelocation;
    const char* DiffuseTexture;
    const char* NormalTexture;
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
        InitalizeComponent();
        
        if (_accel)
        {
            m_Accelerator = _accel;
        }
        else
        {
            std::cout << "The Accelerator cannot be null" << std::endl;
        }
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

