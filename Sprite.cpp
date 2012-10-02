#include "Sprite.h"
#include "util.h"

Sprite::Sprite(ID2D1Bitmap *img)
	: bitmap(img)
{}

Sprite::~Sprite(void)
{
	SafeRelease(&bitmap);
}

void Sprite::setBitmap(ID2D1Bitmap *img)
{
	SafeRelease(&bitmap);
	bitmap = img;
}

HRESULT Sprite::draw(ID2D1RenderTarget *renderTarget)
{
	renderTarget->DrawBitmap(bitmap);
	return S_OK;
}