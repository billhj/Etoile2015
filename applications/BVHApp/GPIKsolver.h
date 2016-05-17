#pragma once
#include "iksolver.h"
class GPIKsolver :
	public IKSolver
{
	VectorX_ m_mu;
public:
	GPIKsolver(void);
	~GPIKsolver(void);
	virtual std::string getIKSolverName() {return "GaussianProcess";}
	virtual void solveOneStep(Skeleton* chain, std::vector<Vector3_>& targets) override;
	void setParameters(VectorX_ mu)
	{
		m_mu = mu;
	}
};

