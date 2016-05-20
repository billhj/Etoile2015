/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file IKSolver.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include <vector>
#include "Skeleton.h"
#include <ctime>
#define USING_ROOT
class IKSolver
{
protected:
	int m_maxTries;
	double m_targetThreshold;
	double m_stepweight;

	double finalDis;
	bool m_activeRoot;
public:
	virtual std::string getIKSolverName() = 0;
	inline IKSolver(int maxTries = 50, double targetThreshold = 0.01)
		:m_maxTries(maxTries),
		m_targetThreshold(targetThreshold), m_stepweight(0.5)
	{ 
		m_activeRoot = false;
	}


	virtual void solveOneStep(Skeleton* chain, const std::vector<Vector3_>& targets) = 0;

	virtual bool solve(Skeleton* chain, const std::vector<Vector3_>& targets)
	{
#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif
		//chain->resetValues();
		chain->update();
		double distance = 0;
		for(int i = 0; i < chain->m_endeffectors.size(); ++i)
		{
			distance += (targets[i] - chain->m_joint_globalPositions[chain->m_endeffectors[i]]).norm();
		}

		int tries = 0;
		while (tries < m_maxTries &&
			(distance > m_targetThreshold) || tries == 0)
		{
			++tries;
			solveOneStep(chain, targets);
			distance = 0;
			for(int i = 0; i < chain->m_endeffectors.size(); ++i)
			{
				distance += (targets[i] - chain->m_joint_globalPositions[chain->m_endeffectors[i]]).norm();
			}
		}

#if( defined( _DEBUG ) || defined( DEBUG ) )
		time = clock() - time;
		double ms = double(time) / CLOCKS_PER_SEC * 1000;
		std::cout<<"timee elapsed: "<<ms<<std::endl;
		std::cout<<"iterations: "<<tries<< " distance: "<<distance<<std::endl;
#endif
		finalDis = distance;
		//chain->update();
		return true;
	}

	double getDistance()
	{
		return finalDis;
	}

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

	void setRootActive(bool bo)
	{
		m_activeRoot = bo;
	}
};