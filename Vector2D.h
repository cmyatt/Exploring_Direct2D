#ifndef VECTOR_2D
#define VECTOR_2D

#include <d2d1.h>
#include <cmath>


class Vector2D
{
public:
	typedef enum {
		BY_X, BY_Y, BY_XY, BY_YX, BY_MAGNITUDE
	} comparison_type;
	
	Vector2D(double i = 0, double j = 0);
	~Vector2D(void);

	int compareTo(Vector2D& v, comparison_type ct) const;	// -1 if this < v, +1 if this > v, 0 if this == v
	D2D1_POINT_2F toPoint() const;

	double dotProduct(const Vector2D& v) const {
		return (x*v.x)+(y*v.y);
	};
	double magnitude() const {
		return sqrt(x*x+y*y);
	};

	// Mutable methods
	Vector2D add(const Vector2D& v) {
		x += v.x;
		y += v.y;
		return *this;
	};
	Vector2D mult(double n) {
		x *= n;
		y *= n;
		return *this;
	};
	Vector2D divide(double n) {
		x /= n;
		y /= n;
		return *this;
	};
	
	// Immutable methods
	static Vector2D add(const Vector2D& v1, const Vector2D& v2) {
		return Vector2D(v1.x+v2.x, v1.y+v2.y);
	};
	static Vector2D mult(const Vector2D& v, double m) {
		return Vector2D(v.x*m, v.y*m);
	};
	static Vector2D div(const Vector2D& v, double m) {
		return Vector2D(v.x/m, v.y/m);
	};

	double x, y;
};

#endif