//Morgan Ruffell, 2021

#pragma once



#include "WorldBase.h"

using namespace DirectX;

/*
	World is what is drawn on the screen, the world is composed of multiple
	types of meshes.

	Static Meshes are meshes with no animation
	Dynamic Meshes are meshes with animation
	Cameras are what is rendering the scene, there is always a viewport camera, that is what is drawn on the screen.

	You can see the scope for growth within my solution.
*/

class World : public WorldBase
{
	XMFLOAT4X4 worldMatrix;

public:

	World()
	{
		SetTag(EngineObjTag::NonEngine);
	}

	~World()
	{
		if (ViewportCamera)
		{
			delete(ViewportCamera);
		}

		if (StaticMeshes.size() > 0)
		{
			StaticMeshes.clear();
		}

		if (DynamicMeshes.size() > 0)
		{
			DynamicMeshes.clear();
		}
	}

public:




public:

	BaseCamera* GetViewportCamera()
	{
		if (ViewportCamera != nullptr)
		{
			return ViewportCamera;
		}
		else
		{
			ViewportCamera = new BaseCamera();
		}
	}

	void SetViewportCamera(BaseCamera* Camera)
	{
		assert(Camera != nullptr);

		if (ViewportCamera != nullptr)
		{
			// Need to solve this, eventually, should probably use smart pointers for the viewport camera
		}
		else
		{
			ViewportCamera = Camera;
		}
	}

public:

	XMFLOAT4X4 GetWorldMatrix()
	{
		return worldMatrix;
	}

public:

	StaticMesh* GetStaticMesh(std::string& Name)
	{
		if (StaticMeshes.contains(Name))
		{
			return StaticMeshes.at(Name);
		}
	}

	DynamicMesh* GetDynamicMesh(std::string& Name)
	{
		if (DynamicMeshes.contains(Name))
		{
			return DynamicMeshes.at(Name);
		}
	}

	BaseCamera* GetCamera(std::string& Name)
	{
		if (Cameras.contains(Name))
		{
			return Cameras.at(Name);
		}
	}



	// Inherited via WorldBase
	virtual void Initalize() override;

	virtual void Update() override;

	virtual void Terminate() override;

};

