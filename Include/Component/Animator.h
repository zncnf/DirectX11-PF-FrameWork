#pragma once
#include "Structure/Component.h"
#include "Asset/AnimationClip.h"

class Animator : public Component
{
public:
	Animator(GameObject* _object);
	virtual ~Animator();

public:
	virtual void Init() override;
	virtual void Update() override;

public:
	void AddAnimationClip(AnimationClip* clip);

private:
	const aiScene* pScene = nullptr;
	aiNode* node = nullptr;
	aiMesh*  pMesh = nullptr;

	std::map<std::string, AnimationClip*> clips;
};

