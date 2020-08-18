#pragma once
#include "Structure/Component.h"
#include "Asset/AnimationController.h"
#include "SkinnedMeshRenderer.h"

class Animator : public Component
{
public:
	Animator(GameObject* _object);
	virtual ~Animator();

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	void AddController(AnimationController* _controller);
	void PlayAnimationWithClipName(std::string clipName);

private:
	AnimationController* controller;

	vector<SkinnedMeshRenderer*> renderers;
	const aiAnimation * m_Animation = nullptr;
	aiMatrix4x4 m_GlobalInverseTransform;

	std::string NodeName;
	float TimeInSeconds;

	AnimationClip* clip;

	bool animationStop;
	bool animationPlaying;

	//UINT FindScaling(double AnimationTime, const aiNodeAnim* pNodeAnim);
	//UINT FindRotation(double AnimationTime, const aiNodeAnim* pNodeAnim);
	//UINT FindPosition(double AnimationTime, const aiNodeAnim* pNodeAnim);

	void CalcInterpolatedScaling(aiVector3D& Out, double AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& Out, double AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, double AnimationTime, const aiNodeAnim* pNodeAnim);

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);

	void ReadNodeHeirarchy(double AnimationTime, const aiNode* pNode, const aiMatrix4x4& ParentTransform);

	void Play(const aiNode * _rootNode);
};

