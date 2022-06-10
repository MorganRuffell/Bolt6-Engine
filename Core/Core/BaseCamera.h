#pragma once

#include <DirectXMath.h>
#include <Windows.h>

using namespace DirectX;

class BaseCamera
{
	BaseCamera();
	~BaseCamera();

public:

	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

private:

	XMFLOAT3 position;
	XMFLOAT3 direction;
	XMFLOAT3 rotation;

public:

	float rotationY;
	float rotationX;

	void SetPosition(XMFLOAT3 pos);
	XMFLOAT3 GetPosition();

public:

	void RotateX(float x);
	void RotateY(float y);

public:

	void SetRotation(XMFLOAT3 rotation);
	virtual XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	void SetProjectionMatrix(float aspectRatio);
};

