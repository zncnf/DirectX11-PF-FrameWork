#include "D3DUtil.h"
#include "Animator.h"

Animator::Animator(GameObject* _object)
{
	gameObject = _object;
}

Animator::~Animator()
{
}

void Animator::Init()
{
	renderers = gameObject->GetComponentsInAllChildren<SkinnedMeshRenderer>();
}

void Animator::Update()
{
}

void Animator::AddController(AnimationController * _controller)
{
	controller = _controller;
}

void Animator::PlayAnimationWithClipName(std::string clipName)
{
	AnimationClip* clip = controller->GetClipWithName(clipName);

	if(clip)
	{
		m_GlobalInverseTransform = clip->pScene->mRootNode->mTransformation.Inverse();
		m_Animation = clip->pScene->mAnimations[0];

		TestPlay(clip->pScene->mRootNode);
	}
}

UINT Animator::FindScaling(float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (UINT i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}

UINT Animator::FindRotation(float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (UINT i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}

UINT Animator::FindPosition(float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	for (UINT i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}

void Animator::CalcInterpolatedScaling(aiVector3D & Out, float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	UINT ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	UINT NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;

	Out = Start + Factor * Delta;
}

void Animator::CalcInterpolatedRotation(aiQuaternion & Out, float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	UINT RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	UINT NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void Animator::CalcInterpolatedPosition(aiVector3D & Out, float AnimationTime, const aiNodeAnim * pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	UINT PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	UINT NextPositionIndex = (PositionIndex + 1);

	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

const aiNodeAnim * Animator::FindNodeAnim(const aiAnimation * pAnimation, const std::string NodeName)
{
	for (UINT i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return nullptr;
}

void Animator::ReadNodeHeirarchy(float AnimationTime, const aiNode * pNode, const aiMatrix4x4 & ParentTransform)
{
	std::string NodeName(pNode->mName.data);

	aiMatrix4x4 NodeTransformation(pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(m_Animation, NodeName);

	if (pNodeAnim) {

		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		aiMatrix4x4 ScalingM;
		aiMatrix4x4::Scaling(Scaling, ScalingM);

		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		aiMatrix4x4 TranslationM;
		aiMatrix4x4::Translation(Translation, TranslationM);

		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;

	for (UINT i = 0; i < renderers.size(); i++)
	{
		if (renderers[i]->boneMapping.find(NodeName) != renderers[i]->boneMapping.end())
		{
			UINT BoneIndex = renderers[i]->boneMapping.at(NodeName);
			const aiMatrix4x4 m = m_GlobalInverseTransform * GlobalTransformation * renderers[i]->boneInfo.at(BoneIndex).boneOffset;
			renderers[i]->boneInfo.at(BoneIndex).finalTransformation = m;
		}
	}

	for (UINT i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void Animator::TestPlay(const aiNode * _rootNode)
{
	aiMatrix4x4 Identity;

	Identity.a2 = Identity.a3 = Identity.a4 = 0;
	Identity.b1 = Identity.b3 = Identity.b4 = 0;
	Identity.c1 = Identity.c2 = Identity.c4 = 0;
	Identity.d1 = Identity.d2 = Identity.d3 = 0;

	Identity.a1 = Identity.b2 = Identity.c3 = Identity.d4 = 1;

	float TicksPerSecond = m_Animation->mTicksPerSecond != 0 ?
		m_Animation->mTicksPerSecond : 25.0f;

	static float TimeInSeconds = 0.f;

	TimeInSeconds += 20.f;

	if (TimeInSeconds >= 120.f)
		TimeInSeconds = 0;

	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, m_Animation->mDuration);

	ReadNodeHeirarchy(TimeInSeconds, _rootNode, Identity);
}
