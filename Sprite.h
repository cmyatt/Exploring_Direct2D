#pragma once

#include <d2d1.h>
#include "Drawable.h"
#include "AppManager.h"

class Sprite :
	public Drawable
{
public:
	Sprite(ID2D1Bitmap *img = NULL);
	~Sprite(void);
	
	void setBitmap(ID2D1Bitmap *img);

	// Override Drawable methods
	virtual HRESULT draw(ID2D1RenderTarget *renderTarget);

private:
	ID2D1Bitmap *bitmap;
};

