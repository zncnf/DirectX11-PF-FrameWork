#include "D3DUtil.h"
#include "Image.h"
#include "Component/RectTransform.h"

Image::Image(GameObject * _object, const WCHAR * _path)
{
	gameObject = _object;
	path = _path;
}

Image::~Image()
{
}

void Image::Init()
{
	/*if (!bitmap)
	{
		ReleaseCOM(bitmap);
	}

	IWICImagingFactory* wicFactory;
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

	IWICBitmapDecoder* decoder;
	IWICBitmapFrameDecode* frame;
	IWICFormatConverter* converter;
	
	wicFactory->CreateDecoderFromFilename(path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
	decoder->GetFrame(0, &frame);
	wicFactory->CreateFormatConverter(&converter);
	converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom);

	DirectXManager::GetInstance()->GetRenderTarget()->CreateBitmapFromWicBitmap(converter, nullptr, &bitmap);
	
	ReleaseCOM(converter);
	ReleaseCOM(frame);
	ReleaseCOM(decoder);
	ReleaseCOM(wicFactory);*/
}

void Image::Update()
{
	/*DirectXManager::GetInstance()->GetRenderTarget()->DrawBitmap(bitmap, &D2D1::RectF(gameObject->rectTransform->position.x - gameObject->rectTransform->width / 2,
		gameObject->rectTransform->position.y - gameObject->rectTransform->height / 2,
		gameObject->rectTransform->position.x + gameObject->rectTransform->width / 2,
		gameObject->rectTransform->position.y + gameObject->rectTransform->height / 2));*/
}
