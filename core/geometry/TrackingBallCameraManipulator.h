/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file TrackingBallCameraManipulator.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "CameraManipulator.h"

namespace Etoile
{
	class TrackingBallCameraManipulator : public CameraManipulator
	{
		
	public:
		bool m_move;
		bool m_zoom;
		bool m_rotate;

		TrackingBallCameraManipulator() : CameraManipulator()
		{
			m_move = m_zoom = m_rotate = false;
		}

		void zoom(float delta);
		void moveOnScreen(float dx, float dy);
		void rotateOnScreen(float p_x, float p_y, float c_x, float c_y);
	};
}