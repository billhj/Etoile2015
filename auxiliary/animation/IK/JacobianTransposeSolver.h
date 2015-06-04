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
		JacobianTransposeSolver(int maxTries = 150, float targetThreshold = 0.005):IKSolver(maxTries, targetThreshold){}
		virtual std::string getIKSolverName(){ return "JacobianTranspose";}
		virtual bool compute(IKChain* chain, Eigen::Vector3f, bool) override;
	};
}
