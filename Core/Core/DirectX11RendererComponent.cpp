#include "DirectX11RendererComponent.h"
#include <assert.h>



DirectX11RendererComponent::DirectX11RendererComponent(GraphicsDevice* Accel)
{
    assert(Accel != nullptr);

    InitalizeRasterizerStates(Accel->m_Accelerator.get());



}

void DirectX11RendererComponent::InitalizeComponent()
{
}

bool DirectX11RendererComponent::TerminateComponent()
{
    return false;
}

void DirectX11RendererComponent::InitalizeRasterizerStates(Accelerator* Accel)
{

    StandardRS = new RasterizerState();
    WireframeRS = new RasterizerState();

    StandardRS->InitalizeState(D3D11_FILL_SOLID, D3D11_CULL_BACK, Accel);
    WireframeRS->InitalizeState(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE, Accel);
}

void DirectX11RendererComponent::DestroyRasterizerStates()
{
    delete StandardRS;
    delete WireframeRS;
}

void DirectX11RendererComponent::BeginFrame(Accelerator* Accel)
{

}

void DirectX11RendererComponent::EndFrame(Accelerator* Accel)
{
}

void DirectX11RendererComponent::Update()
{
}

void DirectX11RendererComponent::Render()
{
}

