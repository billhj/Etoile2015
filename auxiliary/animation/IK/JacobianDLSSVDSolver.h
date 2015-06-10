/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianDLSSVDSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"

namespace Etoile
{

	class JacobianDLSSVDSolver : public IKSolver
	{
		float m_dampling_max;
	public:
		JacobianDLSSVDSolver(IKChain* chain, int maxTries = 150, float targetThreshold = 0.005, float stepweight= 0.5, float dampling = 2):IKSolver(chain, maxTries, targetThreshold, stepweight),
		m_dampling_max(dampling){}
		virtual std::string getIKSolverName(){ return "JacobianDLSSVDSolver";}
		virtual bool solve(Eigen::Vector3f, bool) override;
		void setDampingMax(float dampling){ m_dampling_max = dampling; }
		float getDampingMax(){return m_dampling_max;}
	};
}
