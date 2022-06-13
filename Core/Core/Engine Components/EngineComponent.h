#pragma once
#include <string>
#include "Object.h"
enum ComponentState
{
	Initialized,
	Running,
	Error,
	Terminate
};

class EngineComponent : public Object
{
	std::string Name;
	ComponentState State;

public:

	std::string& GetComponentName()
	{
		return Name;
	}

	void SetComponentName(std::string& name)
	{
		Name = name;
	}

public:

	ComponentState GetComponentState()
	{
		return State;
	}

	void SetComponentState(ComponentState newState)
	{
		State = newState;
	}

protected:

	virtual void InitalizeComponent() = 0;
	virtual bool TerminateComponent() = 0;
};

