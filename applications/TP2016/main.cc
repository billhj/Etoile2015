#include <SDL.h>
#include <iostream>
#include "SimulEnv.h"
#include "BoxCollider.h"
#include <time.h>

void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

int main(int argc, char **argv)
{
	std::cerr << "to quit, type C-c in the terminal" << std::endl;
	// srand(time(0));
	int w = 500;
	int h = 500;
	Screen screen(w, h);
	int ballNum = 25;
	std::vector<Ball*> balls;
	for(int i = 0; i < ballNum; ++i)
	{
		balls.push_back(new Ball(rand()* i %(w - 50) , rand() * i%(h - 50), rand()%50 / 1500.0, rand()%50 / 1800.0, rand()%30, rand()%255,rand()%255,rand()%255));
	}
	BoxCollider* col = new BoxCollider(rand() %(w - 20) , rand() %(h - 20), 100, 80 ,120,111, 10);
	BoxCollider* col2 = new BoxCollider(rand() %(w - 20) , rand() %(h - 20), 90, 40 ,120, 30, 10);
	SimulEnv simE;
	simE.addBalls(balls);
	simE.addCollider(col);
	simE.addCollider(col2);
	//simE.setFriction(0.0001);
	//simE.setGravity(-0.00001, 0.00001);

	int time = 0;
	while (1)
	{
		//sleep(50);
		simE.update(&screen);
		simE.draw(&screen);
		if(time == 10000)
		{
			/*float random1 = (rand() % 100 - 50) / 50.0;
			float gx= -0.00001 * random1;
			float random2 = (rand() % 100 - 50) / 50.0;
			float gy= 0.00001 * random2;
			simE.setGravity(gx, gy);
			std::cout<<"change gravity: "  <<gx <<" "<< gy <<std::endl;*/
			time = 0;
		}
		++time;
		screen.flip();
	}
}

