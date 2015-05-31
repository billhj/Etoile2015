/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TrackingBallCameraManipulator.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "CameraManipulator.h"
#include "math/MathHead.h"

namespace Etoile
{
	class TrackingBallCameraManipulator : public CameraManipulator
	{
		
	public:
		bool m_move;
		bool m_zoom;
		bool m_rotate;
		bool m_spin_active;
		float m_speed;
		//bool m_spin_started;

		Quaternionf m_spin;

		TrackingBallCameraManipulator() : CameraManipulator()
		{
			m_move = m_zoom = m_rotate = m_spin_active = false;
			m_speed = 1;
		}

		void zoom(float delta);
		void moveOnScreen(float dx, float dy);
		void rotateOnScreen(float p_x, float p_y, float c_x, float c_y);
		void rotateWithOnePoint(Quaternionf rot, Vec3f point);
		void setSpinActive(bool active);
		//void startSpin(float p_x, float p_y, float c_x, float c_y);
		void spinUpdate();
	};
}