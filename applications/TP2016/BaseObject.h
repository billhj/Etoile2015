#ifndef BASE_H_
#define BASE_H_
#include "screen.h"

class BaseObject
{
protected:
	float m_x; 
	float m_y;
	float m_v_x;
	float m_v_y;
	float m_r;
	float m_g;
	float m_b;


	float m_friction;
	float m_gravity_X;
	float m_gravity_Y;
public:
	BaseObject(float x = 0, float y = 0, float v_x = 0, float v_y = 0, float r = 0.5, float g = 0.5, float b = 0.5);
	~BaseObject(void);
	void setPosition(float x = 0, float y = 0);
	float x(){return m_x;}
	float y(){return m_y;}
	void setColor(float r = 0.5, float g = 0.5, float b = 0.5);
	void color(float &r, float &g, float &b)
	{
		r = m_r;
		g = m_g;
		b = m_b;
	}
	void setSpeed(float x = 0, float y = 0);
	void setFriction(float f);
	void setGravity(float gx, float gy);
	float speed_x(){return m_v_x;}
	float speed_y(){return m_v_y;}
	virtual void update(Screen*);
	virtual void draw(Screen*) = 0;
	void updatePosition();
	void updateSpeed();
};

#endif