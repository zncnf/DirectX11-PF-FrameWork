#include "D3DUtil.h"
#include "AnimationController.h"

AnimationController::AnimationController()
{
}

AnimationController::~AnimationController()
{
}

void AnimationController::AddAnimationClip(AnimationClip * clip)
{
	clips.insert(make_pair(clip->clipName, clip));
}

AnimationClip* AnimationController::GetClipWithName(std::string clipName)
{
	if (clips.find(clipName) != clips.end())
	{
		return clips.find(clipName)->second;
	}

	return nullptr;
}

void AnimationController::ShowClipList()
{
	std::map<std::string, AnimationClip*>::iterator iter;

	for (iter = clips.begin(); iter != clips.end(); ++iter)
	{
		cout << iter->first << endl;
	}
}
