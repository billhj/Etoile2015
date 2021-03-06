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
		JacobianTransposeSolver(int maxTries = 150, double targetThreshold = 0.005, double stepweight= 0.5):IKSolver(maxTries, targetThreshold, stepweight){}
		virtual std::string getIKSolverName(){ return "JacobianTranspose";}
		virtual void solveOneStep(IKChain*,Vector3_, bool) override;
		virtual void solveOneStep(IKTree* chain, std::vector<Vector3_> targets, bool enableConstraints)override;
	};
}
