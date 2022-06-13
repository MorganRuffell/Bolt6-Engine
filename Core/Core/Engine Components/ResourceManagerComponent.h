#pragma once
#include "EngineComponent.h"
#include <Core/Core/FBXComponent.h>
#include <iostream>

/*
    By far the most important class in my solution, 


*/

class ResourceManagerComponent : public EngineComponent
{
public:

    ResourceManagerComponent()
    {
        InitalizeComponent();
    }

protected:

    FBXComponent* m_FBXImportComponent;


    // Inherited via EngineComponent
    virtual void InitalizeComponent();

    virtual bool TerminateComponent();

};

