#pragma once

#include "EngineComponent.h"
#include <Core/Core/Keyboard.h>
#include <Core/Core/Mouse.h>



class InputComponent : public EngineComponent
{
	void InitalizeComponent();

	bool TerminateComponent();


	Keyboard*		m_Keyboard;
	Mouse*			m_Mouse;
};	

