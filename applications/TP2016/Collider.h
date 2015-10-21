#ifndef COLLIDER_H_
#define COLLIDER_H_
#include "Ball.h"

class Collider
{
public:
	Collider(void);
	~Collider(void);
	virtual void collideWithBall(Ball* ball) = 0;
	virtual void draw(Screen*) = 0;
};

#endif