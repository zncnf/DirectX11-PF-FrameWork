#pragma once
#include "Structure/Component.h"
class Text : public Component
{
public:
	Text(GameObject* _object, const WCHAR* fontname, WCHAR* sentence, float fontsize, D2D1::ColorF color);
	virtual ~Text();

public:
	virtual void Init() override;
	virtual void Update() override;
	
public:
	void SetColor(D2D1::ColorF color);

	void ShowRectForDebug();
	void OnDebugRect();
	void OffDebugRect();

private:
	WCHAR*  wszText;
	const WCHAR* fontName;
	UINT32    cTextLength;
	IDWriteTextFormat*    pTextFormat = nullptr;
	ID2D1SolidColorBrush* pBrush = nullptr;

	bool debugRect = false;
};

