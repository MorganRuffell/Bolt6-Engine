#include "TestEngine.h"

#include "../../../backends/imgui.h"
#include "../../../backends/imgui_impl_win32.h"
#include "../../../backends/imgui_impl_dx11.h"
#include <cassert>

bool TestEngine::Init(HWND window)
{
    SetState(EngineState::Init);

    m_GraphicsDevice = new GraphicsDevice();

    if (!m_GraphicsDevice->CreateDevice(window))
    {
        m_GraphicsDevice->CleanupDevice();
        return false;
    }

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(m_GraphicsDevice->GetDevice(), m_GraphicsDevice->GetDeviceContext());

    return true;
}

//Called first, prepares all the data on the backbuffer
bool TestEngine::Update()
{
    SetState(EngineState::Updating);


    

    DrawImGui();
    ImGui::Render();


    return true;
}

//Called second, dispatches that data to the screen :)
void TestEngine::Render()
{
    SetState(EngineState::Rendering);

    RenderScene();


    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

    m_GraphicsDevice->GetDeviceContext()->OMSetRenderTargets(1, &m_GraphicsDevice->MainRenderTargetView, NULL);
    m_GraphicsDevice->GetDeviceContext()->ClearRenderTargetView(m_GraphicsDevice->GetMainRenderTargetView(), clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_GraphicsDevice->GetSwapChain()->Present(1, 0);

}

void TestEngine::Terminate()
{
    SetState(EngineState::Terminating);

    delete(m_GraphicsDevice);

    //DestroyImGui(WindowData.Window);
}



void TestEngine::SetupImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
}

//Expand upon this so that he's got data about his joint chain, but lets get to rendering it first.
void TestEngine::DrawImGui()
{
    bool VSyncSetting = true;

    if (VSyncSetting = false)
    {
        UseVsync = 0;
    }
    else
    {
        UseVsync = 1;
    }
    
    bool NoTick = false;

    bool ShowDemoWindow = true;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(11.0f, 11.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(7.0f, 10.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(7.0f, 10.0f));

    if(ImGui::BeginMainMenuBar())
    {

        if (ImGui::BeginMenu("File"))
        {

            ImGui::Separator();

            if (ImGui::MenuItem("Exit", "Alt + F4")) {

                ImGui::End();
            }


            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::BeginMenu("Graphics"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0, 4.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 4.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(2.0f, 4.0f));

                if (ImGui::BeginMenu("Rendering Features"))
                {
                    ImGui::RadioButton("VSync", VSyncSetting);

                    ImGui::EndMenu();

                }

                if (ImGui::BeginMenu("Rendering API"))
                {
                    ImGui::RadioButton("DirectX 11",true);

                    ImGui::EndMenu();

                }


                ImGui::PopStyleVar();
                ImGui::PopStyleVar();
                ImGui::PopStyleVar();

                ImGui::EndMenu();

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("About"))
        {
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();


    if (ImGui::Begin("Details", &NoTick,  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        if(ImGui::BeginChild("Outliner", { 535.0f, 400.0f }, true, ImGuiWindowFlags_None))
        {
            ImGui::SetWindowFontScale(1.2f);

            if (ImGui::BeginTabBar("Top Part of Details"))
            {
                ImGui::SetWindowFontScale(1.0f);

                if (ImGui::BeginTabItem(" Scene Outliner "))
                {

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem(" Display Settings "))
                {

                    ImGui::EndTabItem();
                }

            }
            ImGui::EndTabBar();

            ImGui::EndChild();
        }

        if (ImGui::BeginChild("Details", { 535.0f, 920.0f }, true, ImGuiWindowFlags_None))
        {
            //Placeholder until actual data
            float BasePosition[3] = { 0.0f, 0.0f, 0.0f };

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 10.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(8.0f, 6.0f));

            if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
            {
                ImGui::Text("Position		  ");

                ImGui::SameLine();
                ImGui::DragFloat("X", BasePosition, 0.1, -360, 360);

                ImGui::SameLine();
                ImGui::DragFloat("Y", BasePosition, 0.1, -360, 360);

                ImGui::SameLine();
                ImGui::DragFloat("Z", BasePosition, 0.1, -360, 360);

                ImGui::Text("Rotation		  ");
                ImGui::SameLine();
                ImGui::DragFloat("X", BasePosition, 0.1, -360, 360);

                ImGui::SameLine();
                ImGui::DragFloat("Y", BasePosition, 0.1, -360, 360);

                ImGui::SameLine();
                ImGui::DragFloat("Z", BasePosition, 0.1, -360, 360);


                ImGui::Text("Scale			 ");
                ImGui::SameLine();
                ImGui::DragFloat("X", BasePosition, 0.1, -360, 360);

                ImGui::SameLine();
                ImGui::DragFloat("Y", BasePosition, 0.1, -360, 360);

                ImGui::SameLine();
                ImGui::DragFloat("Z", BasePosition, 0.1, -360, 360);


                ImGui::Text("Translation       ");

                ImGui::SameLine();
                ImGui::RadioButton("Local", false);

                ImGui::SameLine();
                ImGui::RadioButton("World", true);

            }

            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();

            if (ImGui::CollapsingHeader("Per Bone Selection Values", ImGuiTreeNodeFlags_DefaultOpen))
            {

            }

            ImGui::EndChild();
        }

        ImGui::End();
    }


    ImGui::Begin("Stats", &NoTick, ImGuiWindowFlags_NoDecorationInvisible);
    {
        ImGui::SetWindowFontScale(1.3f);

        ImGui::TextWrapped("Engine Info");
        ImGui::SetWindowFontScale(1.0f);

        ImGui::TextWrapped("Rendering API: DirectX 11.1 ");
        ImGui::TextWrapped("Developer: Morgan Ruffell");

        ImGui::Separator();

        ImGui::TextWrapped("Date Started: 09/06/2022");
        ImGui::TextWrapped("Date Finished: --");
        ImGui::TextWrapped("Programming Test: Assessed by CTO of Bolt6.ai");


        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::SetWindowFontScale(1.3f);

        std::string DisplayTimeBetweenFrames = std::to_string(ImGui::GetIO().DeltaTime);
        const char* CharRepresentationA = DisplayTimeBetweenFrames.c_str();
        ImGui::SameLine();
        ImGui::TextWrapped("Deltatime :");

        ImGui::SameLine();
        ImGui::Text(CharRepresentationA);


        std::string FrameRate = std::to_string(ImGui::GetIO().Framerate);
        const char* FrameRateRepresentation = FrameRate.c_str();

        ImGui::TextWrapped("  FPS :");


        if (ImGui::GetIO().Framerate > 60.0f)
        {
            ImGui::SameLine();
            ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, FrameRateRepresentation);
        }
        else
        {
            ImGui::SameLine();
            ImGui::TextColored({ 1.0f, 1.0f, 0.0f, 1.0f }, FrameRateRepresentation);

        }

        ImGui::SetWindowFontScale(1.0f);

    }
    ImGui::End();


    ImGui::ShowDemoWindow(&ShowDemoWindow);

}

bool TestEngine::DestroyImGui()
{

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();


    return true;
}



void TestEngine::InitalizeScene()
{

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

