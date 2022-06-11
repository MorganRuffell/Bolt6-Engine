#pragma once
#include "TestEngineBase.h"
#include <Core/Core/UIComponent.h>
#include "Core/Core/Engine/Graphics/Accelerator.h"
#include <Core/Core/GraphicsComponent.h>
#include "Singleton.h"
#include <Core/Core/DirectX11RendererComponent.h>
#include <Core/Core/InputComponent.h>


/// <summary>
/// This is the test engine class, this contains all of the information about the engine current state.
/// This was developed as a test for Bolt6 from Morgan Ruffell, use at your lesiure!
/// </summary>
class TestEngine : public TestEngineBase, public Singleton
{
public:

    std::string ApplicationName = "DirectX 11 Animation Application";
    int UseVsync = 1;

    TestEngine()
        :TestEngineBase()
    {
        State = EngineState::PreInit;
    }

    //This utilizes Dependency injection to inject new instances of Engine Components
    //and build the TestEngine itself

public:

    GraphicsDevice*         m_GraphicsDevice;

public:

    GraphicsComponent*              m_GraphicsComponent;
    DirectX11RendererComponent*     m_RendererComponent;
    UIComponent*                    m_UIComponent;
    InputComponent*                 m_InputComponent;

public:

    // Inherited via TestEngineBase
    virtual bool Init(HWND window) override;

    virtual bool Update() override;

    virtual void Render() override;

    virtual void Terminate() override;

public:

    void InitalizeScene();
    void DrawScene();
    void RenderScene();


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
