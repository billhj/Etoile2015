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
		double m_targetThreshold;
		double m_stepweight;
		IKChain * p_chain;
	public:
		virtual std::string getIKSolverName() = 0;
		inline IKSolver(IKChain* chain, int maxTries = 50, double targetThreshold = 0.005, double stepweight= 0.5)
			: p_chain(chain), m_maxTries(maxTries),
			m_targetThreshold(targetThreshold), m_stepweight(stepweight)
		{ 
		}

		virtual bool solve(Vector3_, bool) = 0;

		inline double getSingleStepValue() const
		{
			return m_stepweight;
		}

		void setSingleStepValue(double v)
		{
			this->m_stepweight = v;
		}

		inline double getTargetThreshold() const
		{
			return m_targetThreshold;
		}

		void setTargetThreshold(double targetThreshold)
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

		double castPiRange(double value)
		{
			while(value > 3.14159265)
			{
				value -= 3.14159265 * 2;
			}
			while(value < -3.14159265)
			{
				value += 3.14159265 * 2;
			}
			return value;
		}

		double clamp(double value, double minV, double maxV)
		{
			if (value > maxV) {
				value -= 3.14159265 * 2;
				if (value < minV){
					value = maxV;
				}
			}
			if (value < minV) {
				value += 3.14159265 * 2;
				if (value > maxV) {
					value = minV;
				}
			}
			return value;
		}
	};
}

