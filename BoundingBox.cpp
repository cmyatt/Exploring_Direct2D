#include "BoundingBox.h"
using namespace std;

BoundingBox::BoundingBox(int level, collision_method m)
	: Poly2D(FILL_POLY),
	  collision_level(level),
	  method(m),
	  position(new Vector2D(50, 50))
{
	setPosition(position);
}

BoundingBox::BoundingBox(const std::vector<Vector2D>& vecs,
						 int level,
						 collision_method m)
	: Poly2D(OUTLINE_POLY),
	  collision_level(level),
	  method(m),
	  position(new Vector2D(50, 50))
{
	addVertices(vecs);
	setPosition(position);
}

BoundingBox::~BoundingBox(void)
{
	if (position)
		delete position;
}

bool BoundingBox::collidesWith(const BoundingBox& b) const
{
	switch (method) {
	case RECT:
		return rectCollision(b);
	case RADIAL:
		return radialCollision(b);
	case SAT:
		return SATCollision(b);
	}
}

void BoundingBox::move(const Vector2D& v)
{
	if (position)
		position->add(v);
}

bool BoundingBox::rectCollision(const BoundingBox& b) const
{
	D2D1_SIZE_F size = getSize();
	D2D1_SIZE_F bSize = b.getSize();

	if (position->x >= b.getPosition().x+bSize.width)
		return false;
	if (position->x+size.width <= b.getPosition().x)
		return false;
	if (position->y >= b.getPosition().y+bSize.height)
		return false;
	if (position->y+size.height <= b.getPosition().y)
		return false;

	return true;
}

bool BoundingBox::radialCollision(const BoundingBox& b) const
{
	return false;
}

bool BoundingBox::SATCollision(const BoundingBox& b) const
{
	return false;
}