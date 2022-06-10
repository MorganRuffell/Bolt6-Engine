#include "BaseCamera.h"

void BaseCamera::SetPosition(XMFLOAT3 pos)
{
}

XMFLOAT3 BaseCamera::GetPosition()
{
    return XMFLOAT3();
}

void BaseCamera::RotateX(float x)
{
}

void BaseCamera::RotateY(float y)
{
}

void BaseCamera::SetRotation(XMFLOAT3 rotation)
{
}

XMFLOAT4X4 BaseCamera::GetViewMatrix()
{
    return XMFLOAT4X4();
}

XMFLOAT4X4 BaseCamera::GetProjectionMatrix()
{
    return XMFLOAT4X4();
}

void BaseCamera::SetProjectionMatrix(float aspectRatio)
{
}
