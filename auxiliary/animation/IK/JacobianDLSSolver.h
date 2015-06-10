/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianDLSSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"

namespace Etoile
{

	class JacobianDLSSolver : public IKSolver
	{
		float m_dampling;
	public:
		JacobianDLSSolver(IKChain* chain, int maxTries = 150, float targetThreshold = 0.005, float stepweight= 0.5, float dampling = 0.9):IKSolver(chain, maxTries, targetThreshold, stepweight),
		m_dampling(dampling){}
		virtual std::string getIKSolverName(){ return "JacobianDLS";}
		virtual bool solve(Eigen::Vector3f, bool) override;
		void setDamping(float dampling){ m_dampling = dampling; }
		float getDamping(){return m_dampling;}
	};
}
