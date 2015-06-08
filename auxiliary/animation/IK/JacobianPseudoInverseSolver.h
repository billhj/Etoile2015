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
		JacobianPseudoInverseSolver(IKChain* chain, int maxTries = 150, float targetThreshold = 0.005, float stepweight= 0.5): IKSolver(chain, maxTries, targetThreshold, stepweight){}
		virtual std::string getIKSolverName(){ return "JacobianPseudoInverse";}
		virtual bool solve(Eigen::Vector3f, bool) override;
		//Eigen::MatrixXf computeRotations(Eigen::MatrixXf jacobian, Eigen::Vector3f dT);
	};
}
