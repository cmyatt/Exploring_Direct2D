#include "Vector2D.h"
#include <cmath>


Vector2D::Vector2D(double i, double j)
	: x(i), y(j)
{}

Vector2D::~Vector2D(void)
{}

int Vector2D::compareTo(Vector2D& v, Vector2D::comparison_type ct) const
{
	double result;
	switch (ct) {
	case BY_X:
		result = x-v.x;
	case BY_Y:
		result = y-v.y;
	case BY_XY:
		if (x-v.x < 0) result = -1;
		if (x-v.x > 0) result = 1;
		result = y-v.y;
	case BY_YX:
		if (y-v.y < 0) result = -1;
		if (y-v.y > 0) result = 1;
		result = x-v.x;
	case BY_MAGNITUDE:
		result = magnitude()-v.magnitude();
	}
	return static_cast<int>(result);
}

D2D1_POINT_2F Vector2D::toPoint() const
{
	return D2D1::Point2F(static_cast<float>(x),
						 static_cast<float>(y));
}