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
		double m_dampling;
	public:
		JacobianDLSSolver(int maxTries = 1000, double targetThreshold = 0.005, double stepweight= 0.5, double dampling = 0.9):IKSolver(maxTries, targetThreshold, stepweight),
		m_dampling(dampling){}
		virtual std::string getIKSolverName(){ return "JacobianDLS";}
		virtual bool solve(IKChain*,Vector3_, bool) override;
		virtual bool solve(IKChain*,Vector3_, Vector3_, bool) override;
		void setDamping(double dampling){ m_dampling = dampling; }
		double getDamping(){return m_dampling;}
	};
}
