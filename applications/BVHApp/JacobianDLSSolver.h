/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianDLSSolver.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include "iksolver.h"
class JacobianDLSSolver : public IKSolver
{
public:
	double m_dampling;
	virtual std::string getIKSolverName() {return "JacobianDLSSolver";}
	JacobianDLSSolver(int maxTries = 50, double targetThreshold = 0.01,double dampling = 0.01): IKSolver(maxTries, targetThreshold),
		m_dampling(dampling){}

	~JacobianDLSSolver(void);
	virtual void solveOneStep(Skeleton* chain, const std::vector<Vector3_>& targets) override;
};

