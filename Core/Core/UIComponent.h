#pragma once
#include "../../../backends/imgui.h"
#include "../../../backends/imgui_impl_win32.h"
#include "../../../backends/imgui_impl_dx11.h"
#include <d3d11.h>
#include <string>
#include <memory>
#include "Core/Core/Engine/Graphics/Accelerator.h"

/*
	This is UI Component a class that will be inside the main engine,
	rather than having a single heavy class I'm going to inject this as a dependency!
*/
class UIComponent
{
public:

	UIComponent(_In_ Accelerator* Accel, _In_ HWND window)
	{
		assert(Accel != nullptr);


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(Accel->GetDevice(), Accel->GetDeviceContext());
	}

	UIComponent(_In_ std::shared_ptr<Accelerator> Accel, _In_ HWND window)
	{
		assert(Accel != nullptr);


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(Accel->GetDevice(), Accel->GetDeviceContext());
	}

	~UIComponent();

	void SetupImGui();
	void DrawImGui();
	void RenderImGui();

	bool DestroyImGui();

public:



};

