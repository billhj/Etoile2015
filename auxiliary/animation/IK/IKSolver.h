/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IKSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <vector>
#include "IKchain.h"
#include <ctime>

namespace Etoile
{
	enum IKSolverType
	{
		CCD,
		JACOBIAN_DLS,
		JACOBIAN_DLSSVD,
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
	public:
		virtual std::string getIKSolverName() = 0;
		inline IKSolver(int maxTries = 50, double targetThreshold = 0.005, double stepweight= 0.5)
			:m_maxTries(maxTries),
			m_targetThreshold(targetThreshold), m_stepweight(stepweight)
		{ 
		}

		virtual bool solve(IKChain* chain, Vector3_ target, bool enableConstraints = true)
		{
#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif
			chain->updateAllDims();
			Vector3_& endpos = chain->m_dim_globalPositions.back();
			Vector3_ distance = (target-endpos);
			int tries = 0;
			while (tries < m_maxTries &&
				(distance.norm() > m_targetThreshold) || tries == 0)
			{
				++tries;
				solveOneStep(chain, target, enableConstraints);
				endpos = chain->m_dim_globalPositions.back();
				distance = (target - endpos);
			}

#if( defined( _DEBUG ) || defined( DEBUG ) )
		time = clock() - time;
		int ms = double(time) / CLOCKS_PER_SEC * 1000;
		std::cout<<"timee elapsed: "<<ms<<std::endl;
		std::cout<<"iterations: "<<tries<< " distance: "<<distance.norm()<<std::endl;
#endif
			chain->update();
			if (distance.norm() > m_targetThreshold)
			{
				return false;
			}
			return true;
		}

		//virtual bool solve(IKChain*, Vector3_, Vector3_, bool) = 0;
		virtual void solveOneStep(IKChain*, Vector3_, bool) = 0;



		virtual bool solve(IKTree* chain, std::vector<Vector3_> targets, bool enableConstraints = true)
		{
#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif
			chain->updateAllDims();
			double distance = 0;
			for(int i = 0; i < chain->m_dim_end_effector_index.size(); ++i)
			{
				distance += (targets[i] - chain->m_dim_globalPositions[chain->m_dim_end_effector_index[i]]).norm();
			}

			int tries = 0;
			while (tries < m_maxTries &&
				(distance > m_targetThreshold) || tries == 0)
			{
				++tries;
				solveOneStep(chain, targets, enableConstraints);
				distance = 0;
				for(int i = 0; i < chain->m_dim_end_effector_index.size(); ++i)
				{
					distance += (targets[i] - chain->m_dim_globalPositions[chain->m_dim_end_effector_index[i]]).norm();
				}
			}

#if( defined( _DEBUG ) || defined( DEBUG ) )
		time = clock() - time;
		int ms = double(time) / CLOCKS_PER_SEC * 1000;
		std::cout<<"timee elapsed: "<<ms<<std::endl;
		std::cout<<"iterations: "<<tries<< " distance: "<<distance<<std::endl;
#endif
			chain->update();
			/*if (distance.norm() > m_targetThreshold)
			{
				return false;
			}*/
			return true;
		}


		virtual void solveOneStep(IKTree*, std::vector<Vector3_>, bool){}

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
				/*value -= 3.14159265 * 2;
				if (value < minV){*/
					value = maxV;
				//}
			}
			if (value < minV) {
				/*value += 3.14159265 * 2;
				if (value > maxV) {*/
					value = minV;
				//}
			}
			return value;
		}
	};
}

