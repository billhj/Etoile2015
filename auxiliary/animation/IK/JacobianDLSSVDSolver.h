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
		double m_dampling_max;
	public:
		JacobianDLSSVDSolver(IKChain* chain, int maxTries = 1000, double targetThreshold = 0.005, double stepweight= 0.5, double dampling = 0.9):IKSolver(chain, maxTries, targetThreshold, stepweight),
		m_dampling_max(dampling){}
		virtual std::string getIKSolverName(){ return "JacobianDLSSVDSolver";}
		virtual bool solve(Vector3_, bool cons = true) override;
		virtual bool solve(Vector3_, Vector3_, bool) override{return true;}
		void setDampingMax(double dampling){ m_dampling_max = dampling; }
		double getDampingMax(){return m_dampling_max;}

		double constraintSolved(double v, double minV, double maxV, double averageV);
	};
}
