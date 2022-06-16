#pragma once

#include "Singleton.h"
#include "TestEngineBase.h"

#include <Core/Core/UIComponent.h>
#include <Core/Core/Engine/Graphics/Accelerator.h>
#include <Core/Core/GraphicsComponent.h>
#include <Core/Core/DirectX11RendererComponent.h>
#include <Core/Core/InputComponent.h>
#include <Core/Core/Object.h>
#include <Core/Core/WorldComponent.h>


/// <summary>
/// This is the test engine class, this contains all of the information about the engine current state.
/// This was developed as a test for Bolt6 from Morgan Ruffell, use at your lesiure!
/// </summary>
class TestEngine : public TestEngineBase, public Singleton, public Object
{
public:

    std::string ApplicationName = "DirectX 11 Animation Application";
    int UseVsync = 1;

    TestEngine()
        :TestEngineBase()
    {

        State = EngineState::PreInit;
        SetTag(EngineObjTag::Engine);

    }

    //This utilizes Dependency injection to inject new instances of Engine Components
    //and build the TestEngine itself

public:

    std::unique_ptr<GraphicsDevice>         m_GraphicsDevice;

public:

    std::unique_ptr<GraphicsComponent>              m_GraphicsComponent;
    std::unique_ptr<DirectX11RendererComponent>     m_RendererComponent;

    std::unique_ptr<WorldComponent>                 m_WorldComponent;

    std::unique_ptr<UIComponent>                    m_UIComponent;
    std::unique_ptr<InputComponent>                 m_InputComponent;

public:

    // Inherited via TestEngineBase
    virtual bool Init(HWND window) override;

    virtual bool Update() override;

    virtual void Render() override;

    virtual void Terminate() override;

public:

    void InitalizeScene();

protected:

    //Was going to use a generic approach, abandoned due to project size.
    /*template<typename T>
    bool SetupComponent(T* Component);

    template<typename T>
    bool TerminateComponent(T* Component);*/

public:

    virtual EngineState GetState() override;
    virtual void SetState(EngineState state) override;

private:

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    int display_w = 0;
    int display_h = 0;
};
