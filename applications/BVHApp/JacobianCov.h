/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianCov.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include "iksolver.h"
//#include "GaussianProcess.h"

class JacobianCov : public IKSolver
{
	
	MatrixX_ m_cov;
	MatrixX_ m_invcov;
	VectorX_ m_mu;
	bool m_defined;
	
	//GaussianProcess m_gp;
	VectorX_ last_state;
public:
	double m_dampling1;
	double m_dampling2;
	virtual std::string getIKSolverName() {return "JacobianMGIK";}
	JacobianCov(int maxTries = 50, double targetThreshold = 0.01, double dampling = 0.01, double dampling2 = 0.01): IKSolver(maxTries, targetThreshold),
		m_dampling1(dampling),m_dampling2(dampling2),m_defined(false)
	{ 
	}

	~JacobianCov(void);
	void setParameters(MatrixX_ incov, VectorX_ mu)
	{
		m_invcov = incov;
		m_mu = mu;
		m_defined = true;
	}
	virtual void solveOneStep(Skeleton* chain, const std::vector<Vector3_>& targets) override;

	//void similarIndex(VectorX_ pos);
};

