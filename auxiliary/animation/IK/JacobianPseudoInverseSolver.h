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
		JacobianPseudoInverseSolver(int maxTries = 150, float targetThreshold = 0.005): IKSolver(maxTries, targetThreshold){}
		virtual std::string getIKSolverName(){ return "JacobianPseudoInverse";}
		virtual bool compute(IKChain* chain, Eigen::Vector3f, bool) override;
		//Eigen::MatrixXf computeRotations(Eigen::MatrixXf jacobian, Eigen::Vector3f dT);
	};
}
