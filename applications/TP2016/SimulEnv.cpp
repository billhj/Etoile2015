#include "SimulEnv.h"


SimulEnv::SimulEnv(void)
{

}


SimulEnv::~SimulEnv(void)
{
}

void SimulEnv::addBalls(const std::vector<Ball*>& balls)
{
	for(unsigned int i = 0; i < balls.size(); ++i)
	{
		m_balls.push_back(balls[i]);
	}
	
}

void SimulEnv::addColliders(const std::vector<Collider*>& col)
{
	for(unsigned int i = 0; i < col.size(); ++i)
	{
		m_colliders.push_back(col[i]);
	}
}

void SimulEnv::addBall(Ball* ball){m_balls.push_back(ball);}
void SimulEnv::addCollider(Collider* col){m_colliders.push_back(col);}

void SimulEnv::update(Screen* screen)
{
	for(unsigned int i = 0; i < m_balls.size(); ++i)
	{
		for(unsigned int j = 0; j < m_colliders.size(); ++j)
		{
			m_balls[i]->collide(m_colliders[j]);
		}
	}

	for(unsigned int i = 0; i < m_balls.size(); ++i)
	{
		for(unsigned int j = i; j < m_balls.size(); ++j)
		{
			m_balls[i]->collide(m_balls[j]);
		}
	}

	for(unsigned int i = 0; i < m_balls.size(); ++i)
	{
		m_balls[i]->update(screen);
	}
}

void SimulEnv::draw(Screen* screen)
{
	for(unsigned int i = 0; i < m_balls.size(); ++i)
	{
		m_balls[i]->draw(screen);
	}

	for(unsigned int i = 0; i < m_colliders.size(); ++i)
	{
		m_colliders[i]->draw(screen);
	}
}

void SimulEnv::setFriction(float f)
{
	m_friction = f;
	for(unsigned int i = 0; i < m_balls.size(); ++i)
	{
		m_balls[i]->setFriction(f);
	}
}

void SimulEnv::setGravity(float gx, float gy)
{
	m_gravity_X = gx;
	m_gravity_Y = gy;
	for(unsigned int i = 0; i < m_balls.size(); ++i)
	{
		m_balls[i]->setGravity(gx, gy);
	}
}