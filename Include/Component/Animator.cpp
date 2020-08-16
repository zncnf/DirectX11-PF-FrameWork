#include "D3DUtil.h"
#include "Animator.h"
#include "GameTimer.h"

Animator::Animator(GameObject* _object)
{
	gameObject = _object;

	TimeInSeconds = 0.f;
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
	clip = controller->GetClipWithName(clipName);

	if(clip)
	{
		m_GlobalInverseTransform = clip->pScene->mRootNode->mTransformation.Inverse();
		m_Animation = clip->pScene->mAnimations[0];		

		Play(clip->pScene->mRootNode);
	}
}

UINT Animator::FindScaling(double AnimationTime, const aiNodeAnim * pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (UINT i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}

UINT Animator::FindRotation(double AnimationTime, const aiNodeAnim * pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (UINT i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}

UINT Animator::FindPosition(double AnimationTime, const aiNodeAnim * pNodeAnim)
{
	for (UINT i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;			
		}
	}

	return 0;
}

void Animator::CalcInterpolatedScaling(aiVector3D & Out, double AnimationTime, const aiNodeAnim * pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	UINT ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	UINT NextScalingIndex = (ScalingIndex + 1);

	//assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;

	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;

	Out = Start + Factor * Delta;
}

void Animator::CalcInterpolatedRotation(aiQuaternion & Out, double AnimationTime, const aiNodeAnim * pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	UINT RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	UINT NextRotationIndex = (RotationIndex + 1);
	//assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);

	Out = Out.Normalize();
}

void Animator::CalcInterpolatedPosition(aiVector3D & Out, double AnimationTime, const aiNodeAnim * pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	UINT PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	UINT NextPositionIndex = (PositionIndex + 1);

	//assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

const aiNodeAnim * Animator::FindNodeAnim(const aiAnimation * pAnimation, const std::string NodeName)
{
	for (UINT i = 0; i < pAnimation->mNumChannels; i++) 
	{
		if (pAnimation->mChannels[i]->mNodeName.C_Str() == NodeName) {
			return pAnimation->mChannels[i];
		}
	}

	return nullptr;
}

void Animator::ReadNodeHeirarchy(double AnimationTime, const aiNode * pNode, const aiMatrix4x4 & ParentTransform)
{
	NodeName = pNode->mName.C_Str();

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

	aiMatrix4x4 GlobalTransformation;

	if (strlen(NodeName.c_str()) == 0)
	{
		GlobalTransformation = ParentTransform;
	}
	else
	{
		GlobalTransformation = ParentTransform * NodeTransformation;
	}

	if (strlen(NodeName.c_str()) != 0)
	{
		for (UINT i = 0; i < renderers.size(); i++)
		{
			if (renderers[i]->boneMapping.find(NodeName) != renderers[i]->boneMapping.end()) // frame ���� �κ�.
			{
				UINT BoneIndex = renderers[i]->boneMapping.at(NodeName);
				const aiMatrix4x4 m = m_GlobalInverseTransform * GlobalTransformation * renderers[i]->boneInfo.at(BoneIndex).boneOffset;
				renderers[i]->boneInfo.at(BoneIndex).finalTransformation = m;

				/*cout << "boneIndex : " << BoneIndex << endl;
				std::cout << "NodeName : " << NodeName << "BoneOffset : " << renderers[i]->boneInfo.at(BoneIndex).boneOffset.a1 << std::endl;*/
			}
		}
	}

	for (UINT i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void Animator::Play(const aiNode * _rootNode)
{
	aiMatrix4x4 Identity;

	TimeInSeconds += GameManager::GetInstance()->gameTimer->DeltaTime() * clip->speed;

	if (TimeInSeconds >= m_Animation->mDuration)
		TimeInSeconds = 0;

	double TimeInTicks = TimeInSeconds * m_Animation->mTicksPerSecond;
	double AnimationTime = fmod(TimeInTicks, m_Animation->mDuration);


	ReadNodeHeirarchy(AnimationTime, _rootNode, Identity);
}