#pragma once
#include "AnimationClip.h"

class AnimationController
{
public:
	AnimationController();
	~AnimationController();

public:
	void AddAnimationClip(AnimationClip* clip);
	void ShowClipList();
public:
	std::map<std::string, AnimationClip*> clips;
};

