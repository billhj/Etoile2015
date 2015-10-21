/*
* Ball.cpp
*
*  Created on: Oct 16, 2015
*      Author: Jing
*/

#include "Ball.h"
#include "Collider.h"

Ball::Ball(float x, float y, float v_x, float v_y, float radius, float r, float g, float b) : BaseObject(x, y, v_x, v_y, r, g, b),  m_radius(radius)
{
	// TODO Auto-generated constructor stub
}

Ball::~Ball() 
{
	// TODO Auto-generated destructor stub
}

void Ball::update(Screen* screen)
{
	updateSpeed();
	if(m_x + m_radius >= screen->w())
	{
		m_v_x = -1 * abs(m_v_x);
	}

	if(m_x - m_radius <= 0)
	{
		m_v_x = abs(m_v_x);
	}

	if(m_y + m_radius >= screen->h())
	{
		m_v_y = -1 * abs(m_v_y);
	}

	if(m_y - m_radius <= 0)
	{
		m_v_y = abs(m_v_y);
	}
	updatePosition();
}

void Ball::draw(Screen* screen)
{
	screen->disc(m_x, m_y, m_radius, m_r, m_g, m_b);
}


/*
*
* https://en.wikipedia.org/wiki/Elastic_collision
**/

void Ball::collide(Ball* ball)
{
	float rSum = this->m_radius + ball->m_radius;
	float dx = this->m_x - ball->m_x;
	float dy = this->m_y - ball->m_y;
	float distance = sqrt(pow(dx, 2) + pow(dy, 2));
	if(distance <= rSum)
	{
		//std::cout<<"collision detected!!";
		//update speed
		float m1 = pow(this->m_radius,3);
		float m2 = pow(ball->m_radius,3);
		float dm = m1 - m2;
		float m_sum = m1 + m2;

		float vx1 = (this->m_v_x * dm + 2 * m2 * ball->m_v_x) / m_sum;
		float vy1 = (this->m_v_y * dm + 2 * m2 * ball->m_v_y) / m_sum;

		float vx2 = (ball->m_v_x * (-dm) + 2 * m1 * this->m_v_x) / m_sum;
		float vy2 = (ball->m_v_y * (-dm) + 2 * m1 * this->m_v_y) / m_sum;

		this->setSpeed(vx1,vy1);
		ball->setSpeed(vx2,vy2);


		//update the pur collision Position
		float dr = (rSum - distance) / rSum;
		this->m_x = this->m_x + dx * dr;
		this->m_y = this->m_y + dy * dr;
		ball->m_x = ball->m_x - dx * dr;
		ball->m_y = ball->m_y - dy * dr;


		//update Collision speed drive position
		this->updatePosition();
		ball->updatePosition();
	}

}

void Ball::collide(Collider* collider)
{
	collider->collideWithBall(this);
}