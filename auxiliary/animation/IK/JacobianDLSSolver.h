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
	public:
		JacobianDLSSolver(IKChain* chain, int maxTries = 50, float targetThreshold = 0.005, float stepweight= 0.5):IKSolver(chain, maxTries, targetThreshold, stepweight){}
		virtual std::string getIKSolverName(){ return "JacobianDLS";}
		virtual bool compute(Eigen::Vector3f, bool) override;
	};
}
