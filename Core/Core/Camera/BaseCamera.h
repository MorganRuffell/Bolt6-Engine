#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include "Object.h"

using namespace DirectX;

//This is used because all viewport cameras are unique_ptrs
//simply transfers data across
struct CameraData
{
	XMFLOAT3 position;
	XMFLOAT3 direction;
	XMFLOAT3 rotation;

	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
};

class BaseCamera : public Object
{
public:
	BaseCamera()
	{
		SetTag(EngineObjTag::NonEngine);
	}
	~BaseCamera()
	{

	}

public:

	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

public:

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

	void Update(float DeltaFrame);

public:

	void SetRotation(XMFLOAT3 rotation);
	virtual XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	void SetProjectionMatrix(float aspectRatio);
};

