#include "BaseObject.h"


BaseObject::BaseObject(float x, float y, float v_x, float v_y, float r, float g, float b) : m_x(x), m_y(y), m_v_x(v_x), m_v_y(v_y), m_r(r), m_g(g), m_b(b)
{
	m_friction = 0;
	m_gravity_X = 0;
	m_gravity_Y = 0;
}


BaseObject::~BaseObject(void)
{
}

void BaseObject::setPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void BaseObject::setColor(float r, float g, float b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}

void BaseObject::setSpeed(float x, float y)
{
	m_v_x = x;
	m_v_y = y;
}


void BaseObject::update(Screen* screen)
{
	updateSpeed();
	updatePosition();
}

void BaseObject::updatePosition()
{
	m_x += m_v_x;
	m_y += m_v_y;
	//std::cout<<m_x <<" "<<m_y<<std::endl;
}

void BaseObject::updateSpeed()
{
	m_v_x -= m_v_x * m_friction;
	m_v_y -= m_v_y * m_friction;

	m_v_x += m_gravity_X;
	m_v_y += m_gravity_Y;
}

void BaseObject::setFriction(float f)
{
	m_friction = f;
}

void BaseObject::setGravity(float gx, float gy)
{
	m_gravity_X = gx;
	m_gravity_Y = gy;
}