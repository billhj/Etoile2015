/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IKSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "IKChain.h"

namespace Etoile
{
	enum IKSolverType
	{
		CCD,
		JACOBIAN_DLS,
		JACOBIAN_PSEUDO_INVERSE,
		JACOBIAN_TRANSPOSE,
		MASS_SPRING
	};

	class IKSolver
	{
	protected:
		int m_maxTries;
		float m_targetThreshold;
		float m_stepweight;
		IKChain * p_chain;
	public:
		virtual std::string getIKSolverName() = 0;
		inline IKSolver(IKChain* chain, int maxTries = 50, float targetThreshold = 0.005, float stepweight= 0.5)
			: p_chain(chain), m_maxTries(maxTries),
			m_targetThreshold(targetThreshold), m_stepweight(stepweight)
		{ 
		}

		virtual bool solve(Eigen::Vector3f, bool) = 0;

		inline float getSingleStepValue() const
		{
			return m_stepweight;
		}

		void setSingleStepValue(float v)
		{
			this->m_stepweight = v;
		}

		inline float getTargetThreshold() const
		{
			return m_targetThreshold;
		}

		void setTargetThreshold(float targetThreshold)
		{
			this->m_targetThreshold = targetThreshold;
		}

		inline int getMaxNumberOfTries() const
		{
			return m_maxTries;
		}

		inline void setMaxNumberOfTries(int tries)
		{
			this->m_maxTries = tries;
		}

		float castPiRange(float value)
		{
			while(value > 3.14159265)
			{
				value -= 3.14159265;
			}
			while(value < -3.14159265)
			{
				value += 3.14159265;
			}
			return value;
		}

		float clamp(float value, float minV, float maxV)
		{
			if(value > maxV)
			{
				value = maxV;
			}
			if(value < minV)
			{
				value = minV;
			}
			return value;
		}
	};
}

