#include "InputComponent.h"

InputComponent::InputComponent(HWND window)
{
	m_Keyboard = new Keyboard();
	m_Mouse = new Mouse(window);

}

void InputComponent::InitalizeComponent()
{
}

bool InputComponent::TerminateComponent()
{
	return false;
}
