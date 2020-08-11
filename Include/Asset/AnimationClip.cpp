#include "D3DUtil.h"
#include "AnimationClip.h"

AnimationClip::AnimationClip(const std::string _clipname, const std::string & filepath)
{
	pScene = aiImportFile(filepath.c_str(), aiProcess_ConvertToLeftHanded | aiProcess_Triangulate);

	pAnimation = pScene->mAnimations[0];
	globalInverseTransform = pScene->mRootNode->mTransformation.Inverse();
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::Init()
{
	for (UINT i = 0; i < pScene->mNumMeshes; i++)
	{
		const aiMesh* pAiMesh = pScene->mMeshes[i];

		for (UINT j = 0; j < pAiMesh->mNumBones; j++)
		{
			UINT boneIndex = 0;
			std::string boneName(pAiMesh->mBones[j]->mName.data);

			if (boneMapping.find(boneName) == boneMapping.end())
			{
				boneIndex = numBones;
				numBones++;
				BoneInfo bi;
				boneInfo.push_back(bi);
				boneInfo[boneIndex].boneOffset = pAiMesh->mBones[i]->mOffsetMatrix;
				boneMapping[boneName] = boneIndex;
			}
			else
			{
				boneIndex = boneMapping[boneName];
			}
		}
	}
}

UINT AnimationClip::FindScaling(float animationTime, const aiNodeAnim * nodeAnim)
{
	for (UINT i = 0; i < nodeAnim->mNumScalingKeys - 1; i++) 
	{
		if (animationTime < (float)nodeAnim->mScalingKeys[i + 1].mTime) 
		{
			return i;
		}
	}

	return 0;
}

UINT AnimationClip::FindRotation(float animationTime, const aiNodeAnim * nodeAnim)
{
	for (UINT i = 0; i < nodeAnim->mNumRotationKeys - 1; i++)
	{
		if (animationTime < (float)nodeAnim->mRotationKeys[i + 1].mTime) 
		{
			return i;
		}
	};

	return 0;
}

UINT AnimationClip::FindPosition(float animationTime, const aiNodeAnim * nodeAnim)
{
	for (UINT i = 0; i < nodeAnim->mNumPositionKeys - 1; i++)
	{
		if (animationTime < (float)nodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	return 0;
}

void AnimationClip::CalcInterpolatedScaling(aiVector3D & out, float animationTime, const aiNodeAnim * nodeAnim)
{
}

void AnimationClip::CalcInterpolatedRotation(aiQuaternion & out, float animationTime, const aiNodeAnim * nodeAnim)
{
}

void AnimationClip::CalcInterpolatedPosition(aiVector3D & out, float animationTime, const aiNodeAnim * nodeAnim)
{
}

const aiNodeAnim * AnimationClip::FindNodeAnim(const aiAnimation * animation, const std::string nodeName)
{
	return nullptr;
}

void AnimationClip::ReadNodeHeirarchy(float animationTime, const aiNode * node, const aiMatrix4x4 & parentTransform)
{
	std::string NodeName(node->mName.data);

	//const aiAnimation* pAnimation = m_pScene->mAnimations[0];

	aiMatrix4x4 NodeTransformation(node->mTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {

		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, animationTime, pNodeAnim);
		aiMatrix4x4 ScalingM;
		aiMatrix4x4::Scaling(Scaling, ScalingM);
		//ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
		aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, animationTime, pNodeAnim);
		aiMatrix4x4 TranslationM;

		aiMatrix4x4::Translation(Translation, TranslationM);
		//TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	aiMatrix4x4 GlobalTransformation = parentTransform * NodeTransformation;

	if (boneMapping.find(NodeName) != boneMapping.end()) {
		UINT BoneIndex = boneMapping.at(NodeName);

		const aiMatrix4x4 m = globalInverseTransform * GlobalTransformation * boneInfo.at(BoneIndex).boneOffset;
		//const aiMatrix4x4 m = m_pBoneInfo->at(BoneIndex).FinalTransformation = GlobalTransformation * m_pBoneInfo->at(BoneIndex).BoneOffset;
		//std::cout << NodeName <<"   " << BoneIndex <<std::endl;
		boneInfo.at(BoneIndex).finalTransformation = m;

		//m_GlobalInverseTransform * GlobalTransformation * m_pBoneInfo->at(BoneIndex).BoneOffset;
	}

	for (UINT i = 0; i < node->mNumChildren; i++) {
		ReadNodeHeirarchy(animationTime, node->mChildren[i], GlobalTransformation);
	}
}

void AnimationClip::TestPlay(const aiNode * rootNode)
{
}
