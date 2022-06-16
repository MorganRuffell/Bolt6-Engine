#include "TestEngine.h"

#include <cassert>
#include <iostream>

bool TestEngine::Init(HWND window)
{
    SetState(EngineState::Init);

    m_GraphicsDevice = new GraphicsDevice();

    if (!m_GraphicsDevice->CreateDevice(window))
    {
        m_GraphicsDevice->CleanupDevice();
        return false;
    }

    std::cout << "Graphics Device Creation Succeeded!" << std::endl;

    m_RendererComponent = new DirectX11RendererComponent(m_GraphicsDevice);

    std::cout << "Renderer Component Creation Succeeded!" << std::endl;

    World* Stage = new World();

    m_WorldComponent = new WorldComponent(Stage, m_GraphicsDevice->m_Accelerator.get());

    std::cout << "World Component Creation Succeeded!" << std::endl;


    InitalizeScene();

    m_UIComponent = new UIComponent(m_GraphicsDevice->m_Accelerator, window);

    std::cout << "UI Component Creation Succeeded!" << std::endl;

    m_InputComponent = new InputComponent(window);

    std::cout << "Input Component Creation Succeeded!" << std::endl;

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);

    return true;
}

//Called first, prepares all the data on the backbuffer
bool TestEngine::Update()
{
    SetState(EngineState::Updating);


    m_UIComponent->DrawImGui();
    m_UIComponent->RenderImGui();

    auto world = m_WorldComponent->GetCurrentWorld();

    m_RendererComponent->Update(world,m_RendererComponent->GetViewportCamera(), m_GraphicsDevice->m_Accelerator.get());
    return true;
}

//Called second, dispatches that data to the screen :)
void TestEngine::Render()
{
    SetState(EngineState::Rendering);

    RenderScene();


    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

    auto Accelerator = m_GraphicsDevice->m_Accelerator;

    Accelerator->GetDeviceContext()->OMSetRenderTargets(1, &m_GraphicsDevice->MainRenderTargetView, NULL);
    Accelerator->GetDeviceContext()->ClearRenderTargetView(m_GraphicsDevice->GetMainRenderTargetView(), clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_RendererComponent->Render(m_GraphicsDevice,1,0);
}

void TestEngine::Terminate()
{
    SetState(EngineState::Terminating);

    delete(m_GraphicsDevice);

    m_UIComponent->DestroyImGui();

    delete(m_UIComponent);
}



void TestEngine::InitalizeScene()
{
    //Heap allocate our new world -- This will contain all of our scene components

 


}

void TestEngine::DrawScene()
{

}

void TestEngine::RenderScene()
{
}

EngineState TestEngine::GetState()
{
    return TestEngine::State;
}

void TestEngine::SetState(EngineState state)
{
    TestEngine::State = state;
}

