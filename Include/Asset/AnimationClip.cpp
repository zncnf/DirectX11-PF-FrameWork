#include "D3DUtil.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip(const std::string _clipname, const std::string & filepath)
{
	pScene = aiImportFile(filepath.c_str(), aiProcess_ConvertToLeftHanded | aiProcess_Triangulate);
	clipName = _clipname;
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::Init()
{
	
}

