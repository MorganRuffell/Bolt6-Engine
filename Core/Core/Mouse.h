// Morgan Ruffell, 2022

#pragma once

#include "InputDeviceBase.h"
#include <iostream>
#include <functional>
#include <wtypes.h>
#include <map>
#include <cassert>
#include <thread>


enum MouseInput
{
	MB_Left,
	MB_Right,
	MB_Middle
};

enum MouseSensitivitySettings
{
	Soft,
	Firm,
	Hard,
};

typedef std::function<void(WPARAM, int, int)>					InputDelegate;

typedef std::vector<std::function<void(WPARAM, int, int) >>	ButtonUpDelegates;
typedef std::vector<std::function<void(WPARAM, int, int) >>	ButtonDownDelegates;
typedef std::vector<std::function<void(WPARAM, int, int) >>	MosueMoveDelegates;
typedef std::vector<std::function<void(float, int, int)  >>	MouseWheelInputDelegates;


class Mouse : public InputDeviceBase
{
protected:

	bool								UseAsync;

	HWND								window;

	std::map<MouseInput, wchar_t>		MouseKeysInputMap;


	ButtonUpDelegates					MouseButtonUp;
	ButtonDownDelegates					MouseButtonDown;
	MosueMoveDelegates					MouseButtonMove;
	MouseWheelInputDelegates			MouseInputDelegate;

protected:

	MouseSensitivitySettings			m_SensitivitySettings;

public:

	Mouse(HWND _window, bool UseAsync)
	{
		assert(window != nullptr);

		SetTag(EngineObjTag::Engine);


		std::thread InitalizeMouseThread{ [&]() {
			MouseKeysInputMap.insert(GeneratePair(MB_Left, VK_LBUTTON));
			MouseKeysInputMap.insert(GeneratePair(MB_Right, VK_RBUTTON));
			SetAsyncStatus(UseAsync);
			m_SensitivitySettings = MouseSensitivitySettings::Firm;
			std::cout << "Mouse Object Initalized" << std::endl;
		} };

		this->window = _window;

		InitalizeMouseThread.join();

	}

	Mouse(HWND _window, bool UseAsync, MouseSensitivitySettings Setting)
	{
		assert(window != nullptr);

		std::thread InitalizeMouseThread{ [&]() {
			MouseKeysInputMap.insert(GeneratePair(MB_Left, VK_LBUTTON));
			MouseKeysInputMap.insert(GeneratePair(MB_Right, VK_RBUTTON));
			SetAsyncStatus(UseAsync);
			m_SensitivitySettings = Setting;
			std::cout << "Mouse Object Initalized" << std::endl;
		} };

		this->window = _window;

		InitalizeMouseThread.join();

	}

	~Mouse()
	{
		DeviceTerminate();
	}

public:

	std::pair< MouseInput, wchar_t> GeneratePair(MouseInput, int);

public:

	bool GetAsyncStatus()
	{
		return UseAsync;
	}

	void SetAsyncStatus(bool UseAsync)
	{
		UseAsync = UseAsync;
	}

public:

	bool IsKeyPressed(_In_ MouseInput _input);

public:

	void OnMouseUp(WPARAM wParam, int x, int y);
	void OnMouseDown(WPARAM wParam, int x, int y);
	void OnMouseMove(WPARAM wParam, int x, int y);

	void OnMouseWheel(float wheelData, int x, int y);

public:

	void BindOnButtonUpCallback(InputDelegate callback);
	void BindOnMouseMoveCallback(InputDelegate callback);
	void BindOnButtonDownCallback(InputDelegate callback);

protected:

	virtual void DeviceInit() override;
	virtual void DeviceUpdate() override;
	virtual void DeviceTerminate() override;
};
