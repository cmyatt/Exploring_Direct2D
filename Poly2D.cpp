#include "Poly2D.h"
#include "util.h"
using namespace std;

Poly2D::Poly2D(D2D1_FIGURE_BEGIN fill)
	: bitmap(NULL), fill_type(fill), position(NULL), brush(NULL), geometry(NULL)
{}

Poly2D::~Poly2D(void)
{
	release();
}

void Poly2D::release() {
	// DO NOT RELEASE start_point HERE
	SafeRelease(&bitmap);
	SafeRelease(&brush);
	SafeRelease(&geometry);
}

void Poly2D::addVertices(const std::vector<Vector2D>& vs)
{
	for (size_t i = 0; i < vs.size(); ++i) {
		vertices.push_back(vs[i]);
	}
}

void Poly2D::addVertex(const Vector2D& v)
{
	vertices.push_back(v);
};

HRESULT Poly2D::create(shared_ptr<AppManager> manager, D2D1::ColorF col)
{
	release();

	HRESULT result = manager->getFactory()->CreatePathGeometry(&geometry);
	if (!SUCCEEDED(result)) return result;

	result = manager->getRenderTarget()->CreateSolidColorBrush(col, &brush);
	if (!SUCCEEDED(result)) return result;

	result = makePath();
	if (!SUCCEEDED(result)) return result;

	ID2D1BitmapRenderTarget *bitmapTarget = NULL;
	result = manager->getRenderTarget()->CreateCompatibleRenderTarget(getSize(), &bitmapTarget);
	if (!SUCCEEDED(result)) return result;

	bitmapTarget->BeginDraw();
	if (fill_type == D2D1_FIGURE_BEGIN_FILLED)
		bitmapTarget->FillGeometry(geometry, brush);
	else if (fill_type == D2D1_FIGURE_BEGIN_HOLLOW)
		bitmapTarget->DrawGeometry(geometry, brush);

	// Draw rect around edge of bitmap
	D2D1_SIZE_U size = bitmapTarget->GetPixelSize();
	D2D1_RECT_F rect;
	rect.left = 0;
	rect.right = size.width;
	rect.top = 0;
	rect.bottom = size.height;
	bitmapTarget->DrawRectangle(&rect, brush);

	result = bitmapTarget->EndDraw();
	if (!SUCCEEDED(result)) {
		SafeRelease(&bitmapTarget);
		return result;
	}

	result = bitmapTarget->GetBitmap(&bitmap);
	SafeRelease(&bitmapTarget);
	return result;
}

HRESULT Poly2D::draw(ID2D1RenderTarget *renderTarget)
{
	if (!bitmap) return E_FAIL;

	D2D1_RECT_F dest;
	D2D1_SIZE_F size = getSize();

	dest.left = position->x;
	dest.right = dest.left + size.width;
	dest.top = position->y;
	dest.bottom = dest.top + size.height;

	renderTarget->DrawBitmap(bitmap, &dest);
	return S_OK;
}

HRESULT Poly2D::makePath()
{
	if (vertices.empty())
		return E_FAIL;

	ID2D1GeometrySink *sink = NULL;
	HRESULT hr = geometry->Open(&sink);
	if (!SUCCEEDED(hr))
		return hr;

	Vector2D min = getMinPoint().mult(-1);
	sink->SetFillMode(D2D1_FILL_MODE_WINDING);
	sink->BeginFigure(min.toPoint(), fill_type);
	
	for (auto i = vertices.cbegin(); i != vertices.cend(); ++i) {
		sink->AddLine(Vector2D::add(*i, min).toPoint());
	}

	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();
	sink->Release();
	return S_OK;
}

Vector2D Poly2D::getMinPoint() const
{
	Vector2D v(vertices[0].x, vertices[0].y);
	for (auto i = vertices.cbegin()+1; i != vertices.cend(); ++i) {
		if (i->x < v.x) v.x = i->x;
		if (i->y < v.y) v.y = i->y;
	}
	return v;
}

Vector2D Poly2D::getMaxPoint() const
{
	Vector2D v(vertices[0].x, vertices[0].y);
	for (auto i = vertices.cbegin()+1; i != vertices.cend(); ++i) {
		if (i->x > v.x) v.x = i->x;
		if (i->y > v.y) v.y = i->y;
	}
	return v;
}

D2D1_SIZE_F Poly2D::getSize() const
{
	Vector2D min=getMinPoint(), max=getMaxPoint();
	return D2D1::SizeF(static_cast<float>(max.x-min.x), static_cast<float>(max.y-min.y));
}