#include "D3DUtil.h"
#include "Execute.h"

Execute::~Execute()
{
	delete camera;
}

void Execute::Init()
{
	SetShaders();
	Set3DObject();
	Set2DObject();
}

void Execute::Update()
{
	Render3D();
	Render2D();
}

void Execute::SetShaders()
{
}

void Execute::Set3DObject()
{
	camera = new GameObject(ObjectType::Empty);
	camera->Init();
}

void Execute::Set2DObject()
{
}

void Execute::Render3D()
{
	camera->Update();
}

void Execute::Render2D()
{
	DirectXManager::GetInstance()->GetRenderTarget()->BeginDraw();
	
	// DrawText Example
	{
		ID2D1SolidColorBrush* pBrush = nullptr;
		IDWriteTextFormat* pTextFormat = nullptr;

		DirectXManager::GetInstance()->GetRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrush);
		DirectXManager::GetInstance()->GetWriteFactory()->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40.0f, L"ko", &pTextFormat);

		pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP); // 줄바꿈 정렬 사용안함

		DirectXManager::GetInstance()->GetRenderTarget()->DrawTextW(L"Text Sample 1", 15, pTextFormat, D2D1::RectF(0, 0, 100, 100), pBrush);

		ReleaseCOM(pBrush);
		ReleaseCOM(pTextFormat);
	}

	DirectXManager::GetInstance()->GetRenderTarget()->EndDraw();
}
