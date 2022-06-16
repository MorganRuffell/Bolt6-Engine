#include "TestEngine.h"

#include <cassert>
#include <iostream>

bool TestEngine::Init(HWND window)
{
    SetState(EngineState::Init);

    m_GraphicsDevice = std::make_unique<GraphicsDevice>();

    if (!m_GraphicsDevice->CreateDevice(window))
    {
        m_GraphicsDevice->CleanupDevice();
        return false;
    }

    std::cout << "Graphics Device Creation Succeeded!" << std::endl;

    m_RendererComponent = std::make_unique<DirectX11RendererComponent>(m_GraphicsDevice.get());

    std::cout << "Renderer Component Creation Succeeded!" << std::endl;

   
    InitalizeScene();

    m_UIComponent = std::make_unique<UIComponent>(m_GraphicsDevice->m_Accelerator, window);

    std::cout << "UI Component Creation Succeeded!" << std::endl;

    m_InputComponent = std::make_unique<InputComponent>(window);

    std::cout << "Input Component Creation Succeeded!" << std::endl;

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);

    return true;
}

//Called first, prepares all the data on the backbuffer
bool TestEngine::Update()
{
    SetState(EngineState::Updating);

    auto world = m_WorldComponent->GetCurrentWorld();

    m_RendererComponent->Update(world,m_RendererComponent->GetViewportCamera(), m_GraphicsDevice->m_Accelerator.get());

    m_UIComponent->DrawImGui();

    return true;
}

//Called second, dispatches that data to the screen :)
void TestEngine::Render()
{
    SetState(EngineState::Rendering);

    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

    auto Accelerator = m_GraphicsDevice->m_Accelerator;

    m_UIComponent->RenderImGui();

    Accelerator->GetDeviceContext()->OMSetRenderTargets(1, &m_GraphicsDevice->MainRenderTargetView, NULL);
    Accelerator->GetDeviceContext()->ClearRenderTargetView(m_GraphicsDevice->GetMainRenderTargetView(), clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_RendererComponent->BeginFrame(m_GraphicsDevice->m_Accelerator.get());
    m_RendererComponent->Render(m_GraphicsDevice.get(), 1, 0);
    m_RendererComponent->EndFrame(m_GraphicsDevice->m_Accelerator.get());
}

void TestEngine::Terminate()
{
    SetState(EngineState::Terminating);

    m_InputComponent.reset();
    m_GraphicsDevice.reset();

    m_UIComponent->DestroyImGui();

    m_WorldComponent.reset();
    m_GraphicsComponent.reset();
    m_RendererComponent.reset();
    m_UIComponent.reset();
}

void TestEngine::InitalizeScene()
{
    //Heap allocate our new world -- This will contain all of our scene components

    World* Stage = new World();

    m_WorldComponent = std::make_unique<WorldComponent>(Stage, m_GraphicsDevice->m_Accelerator.get());

    std::cout << "World Component Creation Succeeded!" << std::endl;
}

EngineState TestEngine::GetState()
{
    return TestEngine::State;
}

void TestEngine::SetState(EngineState state)
{
    TestEngine::State = state;
}

