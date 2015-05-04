/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file Manipulator.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "Component.h"

namespace Etoile
{

	class Manipulator : public Component
	{

	public:
		Manipulator() : Component(MANIPULATOR_COMPONENT)
		{}
	};
}