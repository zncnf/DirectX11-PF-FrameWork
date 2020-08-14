#pragma once
#include "AnimationClip.h"

class AnimationController
{
public:
	AnimationController();
	~AnimationController();

public:
	void AddAnimationClip(AnimationClip* clip);
	AnimationClip* GetClipWithName(std::string clipName);
	void ShowClipList();
public:
	std::map<std::string, AnimationClip*> clips;
};

