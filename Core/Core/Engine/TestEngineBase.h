#pragma once

/*
    This is test engine base, an abstract class that contains the application
    life cycle stages.
*/

#include <d3d11.h>
#include <tchar.h>
#include <stdio.h>

#include "../../../backends/imgui.h"
#include "../../Core/GraphicsDevice.h"

#include <string>

enum EngineState
{
    PreInit,
    Init,
    Rendering,
    Updating,
    Terminating,
    Idle
};

class TestEngineBase
{
protected:


    EngineState State;

public:

    virtual EngineState GetState() = 0;

    virtual void SetState(EngineState state) = 0;

public:
    virtual bool Init(HWND window) = 0;

    virtual bool Update() = 0;

    virtual void Render() = 0;

    virtual void Terminate() = 0;
};

