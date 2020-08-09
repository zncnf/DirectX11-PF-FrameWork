#include "D3DUtil.h"
#include "Animator.h"

Animator::Animator(GameObject* _object, const aiScene * _pScene, aiNode * _node)
{
	gameObject = _object;
	pScene = _pScene;
	node = _node;
}

Animator::~Animator()
{
}

void Animator::Init()
{
	

}

void Animator::Update()
{
}