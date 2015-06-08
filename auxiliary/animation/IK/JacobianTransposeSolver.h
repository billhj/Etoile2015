/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianTransposeSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"

namespace Etoile
{
	class JacobianTransposeSolver : public IKSolver
	{
	public:
		JacobianTransposeSolver(IKChain* chain, int maxTries = 150, float targetThreshold = 0.005, float stepweight= 0.5):IKSolver(chain, maxTries, targetThreshold, stepweight){}
		virtual std::string getIKSolverName(){ return "JacobianTranspose";}
		virtual bool compute(Eigen::Vector3f, bool) override;
	};
}
