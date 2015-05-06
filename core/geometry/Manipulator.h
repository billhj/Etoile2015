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
	protected:
		bool m_active;
		// Sensitivity
		float m_rotSensitivity;
		float m_transSensitivity;
		float m_spinningSensitivity;
		float m_wheelSensitivity;
	public:
		Manipulator() : Component(MANIPULATOR_COMPONENT)
		{
			setRotationSensitivity(1.0f);
			setTranslationSensitivity(1.0f);
			setSpinningSensitivity(0.3f);
			setWheelSensitivity(1.0f);
		}
		void setActive(bool active){m_active = active;}
		bool isActived(){return m_active;}
		void setRotationSensitivity(float sensitivity) { m_rotSensitivity = sensitivity; };
		float rotationSensitivity() const { return m_rotSensitivity; };
		void setTranslationSensitivity(float sensitivity) { m_transSensitivity = sensitivity; };
		float translationSensitivity() const { return m_transSensitivity; };
		void setSpinningSensitivity(float sensitivity) { m_spinningSensitivity = sensitivity; };
		float spinningSensitivity() const { return m_spinningSensitivity; };
		void setWheelSensitivity(float sensitivity) { m_wheelSensitivity = sensitivity; };
		float wheelSensitivity() const { return m_wheelSensitivity; };
	};
}