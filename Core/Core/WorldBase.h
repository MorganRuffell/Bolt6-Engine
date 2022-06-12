#pragma once

#include <map>
#include <string>
#include <Core/Core/StaticMesh.h>
#include <Core/Core/DynamicMesh.h>
#include <Core/Core/BaseCamera.h>


/*
*	Base class for the world class, just removes a lot of the underlying data to
*   this parent.
* 
*	Also allows for further specialization!
*/

class WorldBase : public Object
{

protected:

	virtual void Initalize() = 0;
	virtual void Update() = 0;
	virtual void Terminate() = 0;



protected:

	BaseCamera* ViewportCamera;

protected:

	std::map <std::string, StaticMesh*  >		StaticMeshes;
	std::map <std::string, DynamicMesh* >		DynamicMeshes;
	std::map <std::string, BaseCamera*  >		Cameras;

};

