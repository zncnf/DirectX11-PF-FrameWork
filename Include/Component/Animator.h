#pragma once
#include "Structure/Component.h"

class Animator : public Component
{
public:
	Animator(GameObject* _object, const aiScene * _pScene, aiNode * _node);
	virtual ~Animator();

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	const aiScene* pScene = nullptr;
	aiNode* node = nullptr;
	aiMesh*  pMesh = nullptr;
};

