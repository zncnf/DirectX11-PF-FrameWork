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
}

void Animator::Update()
{
}

void Animator::AddController(AnimationController * _controller)
{
	controller = _controller;
}
