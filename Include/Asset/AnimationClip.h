#pragma once

class AnimationClip
{
public:
	AnimationClip(const std::string _clipname, const std::string & filepath);
	~AnimationClip();

public:
	const aiScene* pScene = nullptr;
	std::string clipName;

public:
	void Init();
};

