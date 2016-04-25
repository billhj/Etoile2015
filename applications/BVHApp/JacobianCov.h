#pragma once
#include "iksolver.h"
class JacobianCov : public IKSolver
{
	double m_dampling;
	MatrixX_ m_cov;
	MatrixX_ m_invcov;
	VectorX_ m_mu;
	bool m_defined;
public:
	virtual std::string getIKSolverName() {return "JacobianCov";}
	JacobianCov(int maxTries = 50, double targetThreshold = 0.05, double stepweight= 0.5, double dampling = 0.1): IKSolver(maxTries, targetThreshold, stepweight),
		m_dampling(dampling),m_defined(false){}

	~JacobianCov(void);
	void setParameters(MatrixX_ incov, VectorX_ mu)
	{
		m_invcov = incov;
		m_mu = mu;
		m_defined = true;
	}
	virtual void solveOneStep(Skeleton* chain, std::vector<Vector3_>& targets) override;
};

