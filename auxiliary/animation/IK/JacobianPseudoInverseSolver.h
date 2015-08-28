/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianPseudoInverseSolver.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "IKSolver.h"
namespace Etoile
{
	class JacobianPseudoInverseSolver : public IKSolver
	{
	public:
		JacobianPseudoInverseSolver(int maxTries = 150, double targetThreshold = 0.005, double stepweight= 0.5): IKSolver(maxTries, targetThreshold, stepweight){}
		virtual std::string getIKSolverName(){ return "JacobianPseudoInverse";}
		virtual void solveOneStep(IKChain*,Vector3_, bool) override;
		virtual void solveOneStep(IKTree*, std::vector<Vector3_>, bool b = true) override;
		//virtual bool solve(IKChain*,Vector3_, Vector3_, bool) override{return true;}
		//Eigen::MatrixX_ computeRotations(Eigen::MatrixX_ jacobian, Vector3_ dT);
	};
}
