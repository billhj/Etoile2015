#pragma once
#include "iksolver.h"
class JacobianTranspose : public IKSolver
{
	double m_dampling;
public:
	virtual std::string getIKSolverName() {return "JacobianTranspose";}
	JacobianTranspose(int maxTries = 50, double targetThreshold = 0.01, double dampling = 0.1): IKSolver(maxTries, targetThreshold),
		m_dampling(dampling){}

	~JacobianTranspose(void);
	virtual void solveOneStep(Skeleton* chain, std::vector<Vector3_>& targets) override;
};

