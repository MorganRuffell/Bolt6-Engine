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
			ViewportCamera.release();
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

public:

	std::vector<StaticMesh*> GetStaticMeshes()
	{
		std::vector<StaticMesh*> Result;

		for (auto& Element : StaticMeshes)
		{
			Result.push_back(Element.second);
		}

		return Result;
	}


	std::vector<DynamicMesh*> GetDynamicMeshes()
	{
		std::vector<DynamicMesh*> Result;

		for (auto& Element : DynamicMeshes)
		{
			Result.push_back(Element.second);
		}

		return Result;
	}


public:

	BaseCamera* GetViewportCamera()
	{
		if (ViewportCamera != nullptr)
		{
			return ViewportCamera.get();
		}
		else
		{
			ViewportCamera = std::make_unique<BaseCamera>();
		}
	}

	void SetViewportCamera(BaseCamera* Camera)
	{
		assert(Camera != nullptr);

		auto NewCamera = ConstructNewViewportCamera(Camera);
		ViewportCamera.swap(NewCamera);
	}


protected:

	std::unique_ptr<BaseCamera> ConstructNewViewportCamera(BaseCamera* Camera)
	{
		assert(Camera != nullptr);

		CameraData Data;

		//Test this, worried about shallow vs deep copies
		Data.position = Camera->GetPosition();
		Data.direction = Camera->direction;
		Data.rotation = Camera->rotation;

		Data.viewMatrix = Camera->GetViewMatrix();
		Data.projectionMatrix = Camera->GetProjectionMatrix();

		delete(Camera);

		std::unique_ptr<BaseCamera> NewCamera = std::make_unique<BaseCamera>();

		NewCamera->direction = Data.direction;
		NewCamera->position = Data.position;
		NewCamera->rotation = Data.rotation;

		NewCamera->viewMatrix = Data.viewMatrix;
		NewCamera->projectionMatrix = Data.projectionMatrix;


		return NewCamera;
	}

public:


	// Inherited via WorldBase
	virtual void Initalize() override;

	virtual void Update() override;

	virtual void Terminate() override;

};

