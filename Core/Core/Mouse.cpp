#include "Mouse.h"

std::pair<MouseInput, wchar_t> Mouse::GeneratePair(MouseInput, int)
{
	return std::pair<MouseInput, wchar_t>();
}

bool Mouse::IsKeyPressed(MouseInput _input)
{
	if (UseAsync)
	{
		if (GetAsyncKeyState(MouseKeysInputMap[_input] & 0x8000) != 0)
		{
			return true;
		}
	}
	else
	{
		if (GetKeyState(MouseKeysInputMap[_input] & 0x8000)!=0)
		{
			return true;
		}

		return false;
	}

	return false;

}

void Mouse::OnMouseUp(WPARAM wParam, int x, int y)
{
	if (MouseButtonUp.size() != 0)
	{
		for (auto cb : MouseButtonUp)
			cb(wParam, x, y);
	}
	else
	{

	}
}

void Mouse::OnMouseDown(WPARAM wParam, int x, int y)
{
	if (MouseButtonUp.size() != 0)
	{
		for (auto cb : MouseButtonUp)
			cb(wParam, x, y);
	}
	else
	{

	}
}

void Mouse::OnMouseMove(WPARAM wParam, int x, int y)
{
	if (MouseButtonDown.size() != 0)
	{
		for (auto cb : MouseButtonDown)
			cb(wParam, x, y);
	}
	else
	{

	}
}

void Mouse::OnMouseWheel(float wheelData, int x, int y)
{
	if (MouseInputDelegate.size() != 0)
	{
		for (auto cb : MouseInputDelegate)
			cb(wheelData, x, y);
	}
	else
	{

	}
}

void Mouse::BindOnButtonUpCallback(InputDelegate callback)
{
	MouseButtonUp.push_back(callback);
}

void Mouse::BindOnMouseMoveCallback(InputDelegate callback)
{
	MouseButtonMove.push_back(callback);
}

void Mouse::BindOnButtonDownCallback(InputDelegate callback)
{
	MouseButtonDown.push_back(callback);
}

void Mouse::DeviceInit()
{

}

void Mouse::DeviceUpdate()
{
}

void Mouse::DeviceTerminate()
{
	MouseKeysInputMap.clear();
	MouseButtonUp.clear();
	MouseButtonDown.clear();
	MouseButtonMove.clear();
	MouseInputDelegate.clear();
}
