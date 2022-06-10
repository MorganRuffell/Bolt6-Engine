#pragma once
#include "TestEngineBase.h"



/// <summary>
/// This is the test engine class, this contains all of the information about the engine current state.
/// This was developed as a test for Bolt6 from Morgan Ruffell, use at your lesiure!
/// </summary>
class TestEngine : public TestEngineBase
{
public:

    std::string ApplicationName = "DirectX 11 Animation Application";
    int UseVsync = 1;

    GraphicsDevice* m_GraphicsDevice;

public:

    void SetupImGui();
    void DrawImGui();
    bool DestroyImGui();

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


public:

    virtual EngineState GetState() override;
    virtual void SetState(EngineState state) override;

private:
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    int display_w = 0;
    int display_h = 0;


    
};

