#include "D3DUtil.h"
#include "RectTransform.h"

RectTransform::RectTransform(GameObject * _object)
{
	gameObject = _object;
	gameObject->transform = this;

	rect = { (LONG)position.x, (LONG)position.y, (LONG)(position.x + 100.0f), (LONG)(position.y + 100.0f) };
}

RectTransform::~RectTransform()
{
}

void RectTransform::Init()
{
	width = 100.0f;
	height = 100.0f;
}

void RectTransform::Update()
{
	rect = { (LONG)position.x, (LONG)position.y, (LONG)(position.x + width), (LONG)(position.y + height) };
}

void RectTransform::SetRectSize(float width, float height)
{
	this->width = width;
	this->height = height;
}
