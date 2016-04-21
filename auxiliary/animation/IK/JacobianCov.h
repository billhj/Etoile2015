/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianCov.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"

namespace Etoile
{

	class JacobianCov : public IKSolver
	{
		double m_dampling;
		MatrixX_ m_cov;
		MatrixX_ m_invcov;
		VectorX_ m_mu;
		bool m_defined;
		std::vector<Vector3_> m_mutarget;
	public:
		JacobianCov(int maxTries = 500, double targetThreshold = 0.005, double stepweight= 0.5, double dampling = 0.01):IKSolver(maxTries, targetThreshold, stepweight),
		m_dampling(dampling),m_defined(false){}
		virtual std::string getIKSolverName(){ return "JacobianCov";}
		virtual void solveOneStep(IKChain*,Vector3_, bool b = true) override;
		virtual void solveOneStep(IKTree*, std::vector<Vector3_>, bool b = true) override;
		//virtual bool solve(IKChain*,Vector3_, Vector3_, bool) override;
		void setDamping(double dampling){ m_dampling = dampling; }
		double getDamping(){return m_dampling;}
	};
}
