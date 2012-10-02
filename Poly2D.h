#pragma once
#ifndef POLY2D_H
#define POLY2D_H

#include <d2d1.h>
#include <vector>
#include <memory>
#include "Vector2D.h"
#include "Drawable.h"
#include "AppManager.h"
#include "Log.h"

#define OUTLINE_POLY	D2D1_FIGURE_BEGIN_HOLLOW
#define FILL_POLY		D2D1_FIGURE_BEGIN_FILLED


class Poly2D
	: public Drawable
{
public:
	Poly2D(D2D1_FIGURE_BEGIN fill = OUTLINE_POLY);
	virtual ~Poly2D(void);
	void release();

	void addVertices(const std::vector<Vector2D>& vs);
	void addVertex(const Vector2D& v);

	void setPosition(Vector2D *v) { position = v; };
	Vector2D getPosition() const { return *position; };
	ID2D1Bitmap *getBitmap() const { return bitmap; };

	// Create the bitmap containing the polygon
	HRESULT create(std::shared_ptr<AppManager> manager, D2D1::ColorF col = D2D1::ColorF::Black);

	// Override Drawable methods
	virtual HRESULT draw(ID2D1RenderTarget *renderTarget);

	D2D1_SIZE_F getSize() const;

private:
	HRESULT makePath();
	Vector2D getMinPoint() const;
	Vector2D getMaxPoint() const;

	ID2D1Bitmap *bitmap;
	D2D1_FIGURE_BEGIN fill_type;
	ID2D1SolidColorBrush *brush;
	ID2D1PathGeometry *geometry;

	Vector2D *position;
	std::vector<Vector2D> vertices;
};

#endif