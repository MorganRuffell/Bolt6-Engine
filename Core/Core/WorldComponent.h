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

	WorldComponent(World* m_World)
	{
		m_CurrentWorld = m_World;
		InitalizeComponent();
	}

public:

	World* GetCurrentWorld()
	{
		return m_CurrentWorld;
	}

protected:

	ResourceManagerComponent*					m_ResourceManagerComponent;

	std::unordered_map<std::string, World*>		Worlds;

	World*										m_CurrentWorld;

	// Inherited via EngineComponent
	virtual void InitalizeComponent();
	virtual bool TerminateComponent();
};

