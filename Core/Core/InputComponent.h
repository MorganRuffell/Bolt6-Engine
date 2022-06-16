#pragma once

#include "EngineComponent.h"
#include <Core/Core/Keyboard.h>
#include <Core/Core/Mouse.h>
#include <wtypes.h>


/*
	Input Component, this is designed to control
	all input in the engine.
*/

class InputComponent : public EngineComponent
{
public:

	InputComponent(HWND window);

public:

	void InitalizeComponent();

	bool TerminateComponent();

public:

	Keyboard* GetKeyboard()
	{
		if (m_Keyboard != nullptr)
		{
			return m_Keyboard;
		}
		else
		{
			return nullptr;
		}
	}

	Mouse* GetMouse()
	{
		if (m_Keyboard != nullptr)
		{
			return m_Mouse;
		}

		else
		{
			return nullptr;
		}
	}

protected:

	Keyboard*		m_Keyboard;
	Mouse*			m_Mouse;
};	

