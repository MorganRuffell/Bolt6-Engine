#include "Keyboard.h"
#include <WinUser.h>

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

bool Keyboard::IsKeyPressed(wchar_t _Key)
{
    return (GetAsyncKeyState(_Key) & 0x8000) != 0;
}

bool Keyboard::IsKeyPressed(wchar_t _Key)
{
	return (GetAsyncKeyState(_Key) & 0x8000) != 0;
}

bool Keyboard::IsKeyPressed(wchar_t _Key)
{
	return (GetAsyncKeyState(_Key) & 0x8000) != 0;
}


bool Keyboard::IsKeyPressed(Keys _Key)
{
	return false;
}

bool Keyboard::IsKeyPressed(FunctionKeys _Key)
{
	return false;
}

bool Keyboard::IsKeyPressed(NumpadKeys _Key)
{
	return false;
}


const Keys Keyboard::GetKeyEnumValue(std::string key)
{
    return Keys();
}

std::vector<Keys> Keyboard::GetKeyEnumValue(std::vector<std::string> keys)
{
    return std::vector<Keys>();
}

void Keyboard::DeviceInit()
{
	std::thread KeysInitThread{ [&]() {

			k_ActionMappings.insert(KeyboardPair("W", Keys::W));
			k_ActionMappings.insert(KeyboardPair("A", Keys::A));
			k_ActionMappings.insert(KeyboardPair("S", Keys::S));
			k_ActionMappings.insert(KeyboardPair("D", Keys::D));
			k_ActionMappings.insert(KeyboardPair("Q", Keys::Q));
			k_ActionMappings.insert(KeyboardPair("E", Keys::E));

			std::cout << "Keyboard keys initalized" << std::endl;
			KeysInit = true;
		} };

	std::thread FunctionKeysInit{ [&]() {

		k_FunctionKeysMappings.insert(FunctionKeyPair("F1", FunctionKeys::F1));
		k_FunctionKeysMappings.insert(FunctionKeyPair("F2", FunctionKeys::F2));
		k_FunctionKeysMappings.insert(FunctionKeyPair("F3", FunctionKeys::F3));
		k_FunctionKeysMappings.insert(FunctionKeyPair("F4", FunctionKeys::F4));
		k_FunctionKeysMappings.insert(FunctionKeyPair("F5", FunctionKeys::F5));

		std::cout << "Function keys initalized" << std::endl;
		FunctionsInit = true;
	} };

	std::thread NumpadKeysInit{ [&]() {

		k_NumpadKeysInputMap.insert(NumpadKeyPair("Left Arrow", NumpadKeys::Left));
		k_NumpadKeysInputMap.insert(NumpadKeyPair("Right Arrow", NumpadKeys::Right));
		k_NumpadKeysInputMap.insert(NumpadKeyPair("Up Arrow", NumpadKeys::Up));
		k_NumpadKeysInputMap.insert(NumpadKeyPair("Down Arrow", NumpadKeys::Down));

		std::cout << "Numpad keys initalized" << std::endl;
		NumpadInit = true;
	} };

}

void Keyboard::DeviceUpdate()
{
}

void Keyboard::DeviceTerminate()
{
}
