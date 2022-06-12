//Morgan Ruffell, 2022

#pragma once

#include "InputDeviceBase.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <thread>

namespace
{

	enum Keys
	{
		Undefined = -1,
		A = 0,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Esc,
		LControl,
		LShift,
		LAlt,
		LSystem,
		RControl,
		RShift,
		RAlt,
		RSystem,
		Menu,
		LBracket,
		RBracket,
		SemiColon,
		Comma,
		Period,
		Quote,
		Slash,
		BackSlash,
		Tilde,
		Equal,
		Dash,
		Space,
		Return,
		BackSpace,
		Tab,
		PageUp,
		PageDown,
		End,
		Home,
		Insert,
		Delete,
		Pause,
	};

	enum FunctionKeys
	{
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
	};

	enum NumpadKeys
	{
		Add,
		Subtract,
		Multiply,
		Divide,
		Left,
		Right,
		Up,
		Down,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
	};


	typedef std::map<std::string, std::vector<Keys>>			KeysInputMap;
	typedef std::map<std::string, std::vector<FunctionKeys>>	FunctionKeysInputMap;
	typedef std::map<std::string, std::vector<NumpadKeys>>		NumpadKeysInputMap;

	typedef std::pair<std::string, Keys>						KeyboardPair;
	typedef std::pair<std::string, FunctionKeys>				FunctionKeyPair;
	typedef std::pair<std::string, NumpadKeys>					NumpadKeyPair;
}


class Keyboard : public InputDeviceBase
{
	KeysInputMap				k_ActionMappings;
	FunctionKeysInputMap		k_FunctionKeysMappings;
	NumpadKeysInputMap			k_NumpadKeysInputMap;

	bool						InitalizationComplete = false;

	bool						KeysInit = false;
	bool						FunctionsInit = false;
	bool						NumpadInit = false;


public:

	Keyboard()
	{
		DeviceInit();
		
		//Attempted this concurrently, not sure if working 100% will debug after second interivew
		while (KeysInit == false && FunctionsInit == false && NumpadInit == false)
		{
			if (KeysInit == true && FunctionsInit == true && NumpadInit == true)
			{
				std::cout << "Keyboard Object Initalized" << std::endl;
				return;
			}
		}
	}

	~Keyboard();


public:

	bool IsKeyPressed(wchar_t _Key);

	bool IsKeyPressed(Keys _Key);
	bool IsKeyPressed(FunctionKeys _Key);
	bool IsKeyPressed(NumpadKeys _Key);


	

	const Keys GetKeyEnumValue(std::string key);
	std::vector<Keys> GetKeyEnumValue(std::vector<std::string> keys);


public:


	// Inherited via InputDeviceBase
	virtual void DeviceInit() override;
	virtual void DeviceUpdate() override;
	virtual void DeviceTerminate() override;
};

