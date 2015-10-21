#ifndef SIM_H_
#define SIM_H_
#include "screen.h"
#include <vector>
#include "Ball.h"
#include "Collider.h"

class SimulEnv
{
private:
	float m_friction;
	float m_gravity_X;
	float m_gravity_Y;
	std::vector<Ball*> m_balls;
	std::vector<Collider*> m_colliders;
public:
	SimulEnv(void);
	~SimulEnv(void);
	void addBalls(const std::vector<Ball*>& balls);
	void addColliders(const std::vector<Collider*>& col);

	void addBall(Ball* ball);
	void addCollider(Collider* col);

	void update(Screen*);
	void draw(Screen*);
	void setFriction(float f);
	void setGravity(float gx, float gy);
};

#endif