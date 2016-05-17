/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianPseudoInverse.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include "iksolver.h"
class JacobianPseudoInverse : public IKSolver
{
	double m_dampling;
public:
	virtual std::string getIKSolverName() {return "JacobianPseudoInverse";}
	JacobianPseudoInverse(int maxTries = 50, double targetThreshold = 0.01, double dampling = 0.1): IKSolver(maxTries, targetThreshold),
		m_dampling(dampling){}

	~JacobianPseudoInverse(void);
	virtual void solveOneStep(Skeleton* chain, const std::vector<Vector3_>& targets) override;
};

