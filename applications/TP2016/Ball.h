/*
 * Ball.h
 *
 *  Created on: Oct 16, 2015
 *      Author: Jing
 */

#ifndef BALL_H_
#define BALL_H_
#include "BaseObject.h"

class Collider;
class Ball : public BaseObject
{
protected:
	float m_radius;
public:
	Ball(float x = 0, float y = 0, float v_x = 0, float v_y = 0, float radius = 1, float r = 0.5, float g = 0.5, float b = 0.5);
	virtual ~Ball();
	virtual void update(Screen*);
	virtual void draw(Screen*);
	void collide(Ball* ball);
	inline float radius(){return m_radius;}
	//visiter mode
	void collide(Collider*);
};

#endif /* BALL_H_ */
