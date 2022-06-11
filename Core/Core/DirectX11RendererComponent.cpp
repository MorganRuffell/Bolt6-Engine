#include "DirectX11RendererComponent.h"
#include <assert.h>

DirectX11RendererComponent::DirectX11RendererComponent(Accelerator* Accel)
{
    assert(Accel != nullptr);

    InitalizeRasterizerStates(Accel);
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
