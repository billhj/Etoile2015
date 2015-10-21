#ifndef BOX_COLLIDER_H_
#define BOX_COLLIDER_H_

#include "Collider.h"
#include "BaseObject.h"
class BoxCollider : public Collider,  public BaseObject
{
protected:
	float m_width;
	float m_height;
public:
	BoxCollider(float x = 0, float y = 0, float w = 5, float h = 5, float r = 0.5, float g = 0.5, float b = 0.5);
	~BoxCollider(void);
	virtual void collideWithBall(Ball* ball);
	virtual void draw(Screen*);
};

#endif