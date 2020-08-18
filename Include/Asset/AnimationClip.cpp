#include "D3DUtil.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip(const std::string _clipname, const std::string & filepath)
{
	pScene = aiImportFile(filepath.c_str(), ASSIMP_LOAD_FLAG_1);
	clipName = _clipname;
	speed = 1.0f;
}

AnimationClip::~AnimationClip()
{
	aiReleaseImport(pScene);
}

void AnimationClip::Init()
{
	
}

