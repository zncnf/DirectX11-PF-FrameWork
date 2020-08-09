#pragma once
#include "Structure/Component.h"

class Image : public Component
{
public:
	Image(GameObject* _object, const WCHAR* _path);
	virtual ~Image();

public:
	virtual void Init() override;
	virtual void Update() override;

private:
	ID2D1Bitmap* bitmap;
	
	const WCHAR* path;
};

