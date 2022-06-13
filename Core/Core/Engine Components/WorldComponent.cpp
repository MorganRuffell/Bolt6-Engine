#include "WorldComponent.h"

void WorldComponent::InitalizeComponent()
{
    m_ResourceManagerComponent = new ResourceManagerComponent();

}

bool WorldComponent::TerminateComponent()
{
    return false;
}
