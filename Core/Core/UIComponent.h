#pragma once

#include "../../../backends/imgui.h"
#include "../../../backends/imgui_impl_win32.h"
#include "../../../backends/imgui_impl_dx11.h"

#include <d3d11.h>
#include <string>
#include <memory>
#include "Core/Core/Engine/Graphics/Accelerator.h"
#include "EngineComponent.h"
#include <Core/Core/ResourceManagerComponent.h>

/*
	This is UI Component a class that will be inside the main engine,
	rather than having a single heavy class I'm going to inject this as a dependency!
	All Engine Components derive from EngineComponent
*/

class UIComponent : public EngineComponent
{
	std::string m_ComponentName = "UI Component";

public:

	UIComponent(_In_ Accelerator* Accel, _In_ HWND window, _In_ World* _world)
	{
		assert(Accel != nullptr);
		SetTag(EngineObjTag::Engine);

		InitalizeComponent();
		SetComponentState(Initialized);
		SetComponentName(m_ComponentName);

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(Accel->GetDevice(), Accel->GetDeviceContext());
		m_World = _world;
	}

	UIComponent(_In_ std::shared_ptr<Accelerator> Accel, _In_ HWND window, _In_ World* _world)
	{
		assert(Accel != nullptr);
		SetTag(EngineObjTag::Engine);


		InitalizeComponent();
		SetComponentState(Initialized);
		SetComponentName(m_ComponentName);

		m_World = _world;

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(Accel->GetDevice(), Accel->GetDeviceContext());
	}

	~UIComponent()
	{
		TerminateComponent();
		SetComponentState(Terminate);
		DestroyImGui();
	}

	void SetupImGui();
	void DrawImGui();
	void RenderImGui();

	bool DestroyImGui();

	World*	m_World;

public:

	// Inherited via EngineComponent
	virtual void InitalizeComponent() override;

	virtual bool TerminateComponent() override;

};

