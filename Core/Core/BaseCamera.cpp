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

void BaseCamera::Update(float DeltaFrame)
{
    float speed = 10.0f;

    XMVECTOR cachedPosition = XMVectorSet(position.x, position.y, position.z,0);
    XMVECTOR cachedDirection = XMVectorSet(direction.x, direction.y, direction.z, 0);

    auto Rotation = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    cachedDirection = XMVector3Rotate(cachedDirection, Rotation);



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
