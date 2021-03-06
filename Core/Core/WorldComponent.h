#pragma once
#include "EngineComponent.h"
#include <Core/Core/World.h>
#include <unordered_map>
#include <Core/Core/ResourceManagerComponent.h>

/*
*   A world component contains all of the worlds that are going to be
*   rendered in the scene. 
* 
*	A world contains all of the scene objects, it's like a scene in unreal.
*
*/

class WorldComponent : public EngineComponent
{
public:

	WorldComponent(World* m_World, Accelerator* _accel)
	{
		m_CurrentWorld = m_World;

		m_ResourceManagerComponent = std::make_shared<ResourceManagerComponent>(_accel, GetCurrentWorld());
	}

	~WorldComponent()
	{
		m_ResourceManagerComponent.reset();
	}

public:

	World* GetCurrentWorld()
	{
		return m_CurrentWorld;
	}

protected:

	std::shared_ptr<ResourceManagerComponent>					m_ResourceManagerComponent;

	std::unordered_map<std::string, World*>						Worlds;

	World*														m_CurrentWorld;

	// Inherited via EngineComponent
	virtual void InitalizeComponent();
	virtual bool TerminateComponent();
};

