#pragma once

class AnimationClip
{
public:
	AnimationClip(const std::string _clipname, const std::string & filepath);
	~AnimationClip();

public:
	const aiScene* pScene = nullptr;
	const aiAnimation* pAnimation = nullptr;

	std::vector<BoneInfo> boneInfo;
	std::map<std::string, UINT> boneMapping;
	aiMatrix4x4 globalInverseTransform;

	UINT numBones = 0;

public:
	void Init();

	UINT FindScaling(float animationTime, const aiNodeAnim* nodeAnim);
	UINT FindRotation(float animationTime, const aiNodeAnim* nodeAnim);
	UINT FindPosition(float animationTime, const aiNodeAnim* nodeAnim);

	void CalcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim);
	void CalcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* nodeAnim);
	void CalcInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim);

	const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string nodeName);

	void ReadNodeHeirarchy(float animationTime, const aiNode* node, const aiMatrix4x4& parentTransform);

	void TestPlay(const aiNode * rootNode);
};

