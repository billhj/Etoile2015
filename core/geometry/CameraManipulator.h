/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file CameraManipulator.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Manipulator.h"

namespace Etoile
{
	class Camera;
	class CameraManipulator : public Manipulator
	{
	protected:
		Camera * p_camera;
	public:
		CameraManipulator() : Manipulator()
		{}
		void setCamera(Camera* camera){ p_camera = camera; }
		Camera* getCamera(){return p_camera;}
	};
}