#include "InputComponent.h"


InputComponent::InputComponent(HWND window)
{
	m_Mouse = new Mouse(window, true);

	InitalizeComponent();
}

void InputComponent::InitalizeComponent()
{
	m_Keyboard = new Keyboard();
}

bool InputComponent::TerminateComponent()
{
	return false;
}
