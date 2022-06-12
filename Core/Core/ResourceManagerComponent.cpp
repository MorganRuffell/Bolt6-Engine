#include "ResourceManagerComponent.h"

void ResourceManagerComponent::InitalizeComponent()
{
    m_FBXImportComponent = new FBXComponent();

    std::cout << "FBX Import Component Initalized Successfully" << std::endl;
    std::cout << "Resource Manager Component Initalized Successfully" << std::endl;
}

bool ResourceManagerComponent::TerminateComponent()
{
    return false;
}
