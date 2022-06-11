#pragma once

#include "InputDeviceBase.h"

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


/*
	One of the lessons I learned with DirectInput was that it blocks absolutely everything.
	Therefore I'm going to make something bespoke.
*/

class Keyboard : public InputDeviceBase
{
public:
	Keyboard();
	~Keyboard();

public:


	// Inherited via InputDeviceBase
	virtual void DeviceInit() override;
	virtual void DeviceUpdate() override;
	virtual void DeviceTerminate() override;
};

