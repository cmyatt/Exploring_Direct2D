#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "Vector2D.h"

/**
 ** This is an interface intended for multiple inheritance.
 ** DO NOT ADD MEMBER VARIABLES.
 **/
class Moveable
{
public:
	virtual ~Moveable();
	virtual void move(const Vector2D& v) = 0;
};


#endif