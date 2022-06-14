
#pragma once
#include <vector>
#include <string>
#include <cassert>

enum FrameRate
{
	Film = 24,
	Games = 30,
};

struct AnimationClip
{
	float m_Length;
	float m_CurrentPlaybackFrame;
	float m_RateScaling = 0;
};

struct AnimationComposition
{
	std::vector<std::string> ClipNames;
	std::vector<AnimationClip*> Clips;
};

class AnimationSequence
{
	AnimationSequence(FrameRate Rate, AnimationClip* AnimClip)
	{
		assert(AnimClip != nullptr);

		m_FrameRate = Rate;
		m_AnimationClip = AnimClip;
	}

	AnimationSequence(FrameRate Rate, AnimationComposition* AnimComposition)
	{
		assert(AnimComposition != nullptr);

		m_FrameRate = Rate;
		m_AnimationComposition = AnimComposition;
	}

	bool Playing;

	AnimationClip* GetAnimationClip()
	{
		return m_AnimationClip;
	}

	AnimationComposition* GetAnimationComposition()
	{
		return m_AnimationComposition;
	}

public:

	FrameRate				m_FrameRate;
	AnimationClip*			m_AnimationClip;

	AnimationComposition*	m_AnimationComposition;
};


