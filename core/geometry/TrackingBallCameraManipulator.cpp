/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TrackingBallCameraManipulator.cpp
* @brief 
* @date 1/2/2011
*/

#include "TrackingBallCameraManipulator.h"
#include "Camera.h"
#include "ModelTransform.h"

namespace Etoile
{
	float tb_project_to_sphere(float r, float x, float y) {
		float d, t, z;
		d = sqrt(x * x + y * y);
		if (d < r * 0.70710678118654752440) {    /* Inside sphere */
			z = sqrt(r * r - d * d);
		}
		else {           /* On hyperbola */
			t = r / 1.41421356237309504880f;
			z = t * t / d;
		}
		return z;
	 /* const float size       = 1.0f;
	  const float size2      = size*size;
	  const float size_limit = size2*0.5;

	  const float d = x*x + y*y;
	  return d < size_limit ? sqrt(size2 - d) : size_limit/sqrt(d);*/
	}

	Quaternionf trackBall(float p1x, float p1y, float p2x, float p2y) {
		//std::cout<<p1x <<" "<<p1y<<" " << p2x<<" " << p2y <<std::endl;
		Quaternionf q;

		if (p1x == p2x && p1y == p2y) {
			/* Zero rotation */
			return q;
		}

		float TRACKBALLSIZE = 1.0f;

		/*
		* First, figure out z-coordinates for projection of P1 and P2 to
		* deformed sphere
		*/
		Vec3f p1(p1x, p1y, tb_project_to_sphere(TRACKBALLSIZE, p1x, p1y));
		Vec3f p2(p2x, p2y, tb_project_to_sphere(TRACKBALLSIZE, p2x, p2y));

		/*
		*  Now, we want the cross product of P1 and P2
		*/
		Vec3f a = p2.cross3(p1);   /* Axis of rotation */
		//a.normalize();
		//std::cout<<"axis:"<<a<<std::endl;
		/*
		*  Figure out how much to rotate around that axis.
		*/
		//Vec3f d = p1 - p2;
		//float t = d.length() / (2.0f * TRACKBALLSIZE);
		const float angle = 2.0 * asin(sqrt(a.length() / p1.length() / p2.length())) / 10;
		/*
		* Avoid problems with out-of-control values...
		*/
		//if (t > 1.0) {
		//	t = 1.0f;
		//}
		//if (t < -1.0) {
		//	t = -1.0f;
		//}

		//float phi = (float)(2.0 * asin(t));  /* how much to rotate about axis */
		q.setAxisAngle(a, angle);

		return q;
	}

	static Vec3f axisY(0,1,0);
	static Vec3f axisX(1,0,0);
	Quaternionf trackBall2(float p1x, float p1y, float p2x, float p2y) {
		//std::cout<<p1x <<" "<<p1y<<" " << p2x<<" " << p2y <<std::endl;
		Quaternionf qX, qY;

		if (p1x == p2x && p1y == p2y) {
			/* Zero rotation */
			return qX;
		}

		float dx = p2x - p1x;
		float dy = p2y - p1y;

		float phiX = (float)(2.0 * asin(dx));  /* how much to rotate about axis */
		float phiY = (float)(2.0 * asin(dy));  /* how much to rotate about axis */
		qX.setAxisAngle(axisY, -phiX);
		qY.setAxisAngle(axisX, -phiY);

		return qX * qY;
	}

	void TrackingBallCameraManipulator::zoom(float delta)
	{

		p_camera->getTransform()->translate(p_camera->getViewDirection() * delta);

	}

	void TrackingBallCameraManipulator::moveOnScreen(float dx, float dy)
	{

		Vec3f trans(dx, -dy, 0.0);
		switch (p_camera->type())
		{
		case CameraType::PERSPECTIVE:
			{
				float length = (p_camera->getTransform()->getPosition() - p_camera->getTarget()).length();
				trans *= 2.0 * tan(p_camera->getFieldOfView() / 2.0) / p_camera->getHeight() * length * translationSensitivity();
				break;
			}
		case CameraType::ORTHOGRAPHIC:
			{
				trans[0] *= 2.0;
				trans[1] *= 2.0;
				break;
			}
		}
		p_camera->getTransform()->translate(p_camera->getTransform()->getOrientation().rotate(trans));

	}

	void TrackingBallCameraManipulator::rotateOnScreen(float p_x, float p_y, float c_x, float c_y)
	{

		Vec3f cameraPos = p_camera->getTransform()->getPosition();
		Vec3f pivot = p_camera->getTarget();
		Quaternionf rot = trackBall(
			(p_x - p_camera->getWidth() * 0.5) / p_camera->getWidth(),
			(p_camera->getHeight() * 0.5 - p_y) / p_camera->getHeight(),
			(c_x - p_camera->getWidth() * 0.5) / p_camera->getWidth(),
			(p_camera->getHeight() * 0.5 - c_y) / p_camera->getHeight());
		p_camera->getTransform()->rotate(rot);

		Vec3f invAxis = p_camera->getTransform()->getOrientation().rotate(rot.axis());
		Quaternionf invRot(invAxis, rot.angle());
		Vec3f trans = invRot.rotate(cameraPos-pivot);
		p_camera->getTransform()->setPosition(pivot + trans);
		
		//std::cout<<p_camera->getTransform()->getOrientation().rotate(Vec3f(0,0,-2)) + p_camera->getTransform()->getPosition()<<std::endl;
	}
}