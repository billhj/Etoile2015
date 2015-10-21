#include "BoxCollider.h"


BoxCollider::BoxCollider(float x, float y, float w, float h, float r, float g, float b): BaseObject(x, y, 0, 0, r, g, b), m_width(w), m_height(h)
{
}


BoxCollider::~BoxCollider(void)
{
}


void BoxCollider::collideWithBall(Ball* ball)
{
	float ballx = ball->x();
	float bally = ball->y();
	float ballr = ball->radius();
	float ballsx = ball->speed_x();
	float ballsy = ball->speed_y();

	//left
	if(ballx + ballr <= m_x + m_width * 0.5 && ballx + ballr >= m_x - m_width * 0.5 /*&& ballx <= m_x*/ && ballx - ballr <= m_x - m_width * 0.5 
		&& ((bally - ballr <=  m_y + m_height * 0.5 && bally + ballr >=  m_y - m_height * 0.5)) )
	{
		ball->setSpeed(-abs(ballsx), ballsy);
	}

	//right
	else if(ballx + ballr >= m_x + m_width * 0.5 && ballx - ballr <= m_x + m_width * 0.5 /*&& ballx >= m_x*/ && ballx - ballr >= m_x - m_width * 0.5
		&& ((bally - ballr <=  m_y + m_height * 0.5 && bally + ballr >=  m_y - m_height * 0.5)) )
	{
		ball->setSpeed(abs(ballsx), ballsy);
	}


	ballsx = ball->speed_x();
	ballsy = ball->speed_y();

	//bottom
	if(bally + ballr >= m_y + m_height * 0.5 && bally - ballr >= m_y - m_height * 0.5 /*&& bally >= m_y*/ && bally - ballr <= m_y + m_height * 0.5
		&& ((ballx - ballr <=  m_x + m_width * 0.5 && ballx + ballr >=  m_x - m_width * 0.5)) )
	{
		ball->setSpeed(ballsx, abs(ballsy));
	}

	//up
	else if(bally + ballr <= m_y + m_height * 0.5 && bally - ballr <= m_y - m_height * 0.5 /*&& bally <= m_y*/ && bally + ballr >= m_y - m_height * 0.5
		&& ((ballx - ballr <=  m_x + m_width * 0.5 && ballx + ballr >=  m_x - m_width * 0.5)) )
	{
		ball->setSpeed(ballsx, -abs(ballsy));
	}

	/*else if(ballx < m_x + m_width * 0.5 && ballx > m_x - m_width * 0.5 && bally < m_y + m_height * 0.5 && bally > m_y - m_height * 0.5)
	{
		if(abs(ballx-m_x) > abs(bally-m_y))
		{
			if(ballx > m_x)
			{
				ball->setSpeed(abs(ballsx), ballsy);
			}else
			{
				ball->setSpeed(-abs(ballsx), ballsy);
			}
		}else
		{
			if(bally > m_y)
			{
				ball->setSpeed(ballsx, abs(ballsy));
			}else
			{
				ball->setSpeed(ballsx, -abs(ballsy));
			}
		}
	
	}*/


	ball->updatePosition();

}

void BoxCollider::draw(Screen* sc)
{
	sc->rect(m_x, m_y, m_width, m_height, m_r, m_g, m_b);
}