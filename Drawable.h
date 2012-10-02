#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <d2d1.h>

/**
 ** This is an interface intended for multiple inheritance.
 ** DO NOT ADD MEMBER VARIABLES.
 **/
class Drawable
{
public:
	virtual ~Drawable();
	virtual HRESULT draw(ID2D1RenderTarget *renderTarget) = 0;
};

#endif