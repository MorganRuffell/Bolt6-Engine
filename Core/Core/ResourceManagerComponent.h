#pragma once
#include "EngineComponent.h"
#include <Core/Core/FBXComponent.h>
#include <iostream>

/*
    By far the most important class in my solution, 


*/

class ResourceManagerComponent : public EngineComponent
{
    ResourceManagerComponent() = delete;
    

public:
    ResourceManagerComponent(Accelerator* _accel)
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

protected:

    int ResourceCountLoaded = 0;

    //I Love making big things... But this is about delivering something quickly.
    void LoadAnimationResources();


protected:

    virtual void LoadResources();
    
    static ResourceManagerComponent* GetInstance()
    {
        return Instance;
    }

    // Inherited via EngineComponent
    virtual void InitalizeComponent();

    virtual bool TerminateComponent();

};

