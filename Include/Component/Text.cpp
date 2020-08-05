#include "D3DUtil.h"
#include "Text.h"
#include "RectTransform.h"

Text::Text(GameObject * _object, const WCHAR* fontname, WCHAR* sentence, float fontsize, D2D1::ColorF color)
{
	gameObject  = _object;
	wszText     = sentence;
	fontName    = fontname;
	cTextLength = (UINT32)wcslen(wszText);

	DirectXManager::GetInstance()->GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(color), &pBrush);
}

Text::~Text()
{
	ReleaseCOM(pTextFormat);
	ReleaseCOM(pBrush);
}

void Text::Init()
{
	DirectXManager::GetInstance()->GetWriteFactory()->CreateTextFormat(fontName, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40.0f, L"ko", &pTextFormat);

	// 텍스트를 왼쪽, 오른쪽, 중간을 기준으로 정렬한다.
	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	// 텍스트를 위쪽, 아래쪽, 중간을 기준으로 정렬한다.
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// 텍스트를 줄바꿈 정렬합니다.
	pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	DirectXManager::GetInstance()->GetRenderTarget()->SetTransform(D2D1::IdentityMatrix());
}

void Text::Update()
{
	DirectXManager::GetInstance()->GetRenderTarget()->DrawTextW(wszText, cTextLength, pTextFormat,
		D2D1::RectF(gameObject->rectTransform->position.x - gameObject->rectTransform->width / 2,
			gameObject->rectTransform->position.y - gameObject->rectTransform->height / 2,
			gameObject->rectTransform->position.x + gameObject->rectTransform->width / 2,
			gameObject->rectTransform->position.y + gameObject->rectTransform->height / 2), pBrush);

	if (debugRect)
		ShowRectForDebug();
}

void Text::SetColor(D2D1::ColorF color)
{
	DirectXManager::GetInstance()->GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(color), &pBrush);
}

void Text::ShowRectForDebug()
{
	DirectXManager::GetInstance()->GetRenderTarget()->DrawRectangle(
		D2D1::RectF(gameObject->rectTransform->position.x - gameObject->rectTransform->width / 2,
			gameObject->rectTransform->position.y - gameObject->rectTransform->height / 2,
			gameObject->rectTransform->position.x + gameObject->rectTransform->width / 2,
			gameObject->rectTransform->position.y + gameObject->rectTransform->height / 2), pBrush);
}

void Text::OnDebugRect()
{
	debugRect = true;
}

void Text::OffDebugRect()
{
	debugRect = false;
}
