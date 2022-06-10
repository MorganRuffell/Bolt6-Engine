#include "Core/Engine/TestEngine.h"
#include <backends/imgui_impl_win32.h>



int main()
{
	WNDCLASSEX WindowsClass = { sizeof(WNDCLASSEX), CS_CLASSDC, NULL, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("Test Engine - Bolt6"), NULL };
	::RegisterClassEx(&WindowsClass);

	HWND window = ::CreateWindow(WindowsClass.lpszClassName, _T("Test Engine - Bolt6"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, WindowsClass.hInstance, NULL);

	TestEngine Engine;

	Engine.Init(window);

	while(Engine.Update())
	{


		Engine.Render();
	}

	Engine.Terminate();
}

