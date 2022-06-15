#pragma once

#include "EngineComponent.h"
#include <Core/Core/Object.h>


/*
	In order to render an animation, we have to time the engine.

	This is to ensure that we get the delta time between frames, 

*/

class TimerComponent : public EngineComponent
{
public:

	TimerComponent()
	{
		SetTag(EngineObjTag::Engine);
		InitalizeComponent();

	}

	~TimerComponent()
	{
		TerminateComponent();
	}

public:

	void Reset();
	void Start();

	void Terminate();
	void Update();

public:

	float EngineTime() const;
	float DeltaTime() const;

protected:

	double mSecondsPerCount;
	double mDeltaTime;

	int mBaseTime;
	int mPausedTime;
	int mStopTime;
	int mPrevTime;
	int mCurrentTime;

	bool mIsStopped;

protected:

	// Inherited via EngineComponent
	virtual void InitalizeComponent();
	virtual bool TerminateComponent();
};

