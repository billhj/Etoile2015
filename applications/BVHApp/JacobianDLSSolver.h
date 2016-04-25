#pragma once
#include "iksolver.h"
class JacobianDLSSolver : public IKSolver
{
	double m_dampling;
public:
	virtual std::string getIKSolverName() {return "JacobianDLSSolver";}
	JacobianDLSSolver(int maxTries = 50, double targetThreshold = 0.05, double stepweight= 0.5, double dampling = 0.01): IKSolver(maxTries, targetThreshold, stepweight),
		m_dampling(dampling){}

	~JacobianDLSSolver(void);
	virtual void solveOneStep(Skeleton* chain, std::vector<Vector3_>& targets) override;
};

