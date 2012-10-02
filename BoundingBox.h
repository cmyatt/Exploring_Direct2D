#pragma once
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <d2d1.h>
#include <vector>
#include "Vector2D.h"
#include "Drawable.h"
#include "Moveable.h"
#include "Poly2D.h"

/*
 * Implement collision detection
 *		--> Enumerate different algorithms
 *			--> Radial based
 *			--> SAT based
 */

class BoundingBox :
	public Poly2D,
	public Moveable
{
public:
	typedef enum {
		RECT, RADIAL, SAT
	} collision_method;

	BoundingBox(int level = 0, collision_method m = RECT);
	BoundingBox(const std::vector<Vector2D>& vecs,
				int level = 0,
				collision_method m = RECT);

	virtual ~BoundingBox(void);

	virtual int getCollisionLevel() const { return collision_level; };
	virtual bool collidesWith(const BoundingBox& b) const;

	void setCentre(const Vector2D& v) { centre = v; };		// anyway to have this automatically updated?
															// create ellipse object vs Poly2D?
	Vector2D getCentre() const { return centre; };

	// Override Moveable methods
	virtual void move(const Vector2D& v);

private:
	bool rectCollision(const BoundingBox& b) const;
	bool radialCollision(const BoundingBox& b) const;
	bool SATCollision(const BoundingBox& b) const;

	Vector2D *position;
	Vector2D centre;
	collision_method method;
	int collision_level;
};

#endif