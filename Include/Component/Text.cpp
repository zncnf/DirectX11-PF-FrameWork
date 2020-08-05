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

	// �ؽ�Ʈ�� ����, ������, �߰��� �������� �����Ѵ�.
	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	// �ؽ�Ʈ�� ����, �Ʒ���, �߰��� �������� �����Ѵ�.
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// �ؽ�Ʈ�� �ٹٲ� �����մϴ�.
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
