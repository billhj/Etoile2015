#pragma once
#include "iksolver.h"
class JacobianPseudoInverse : public IKSolver
{
	double m_dampling;
public:
	virtual std::string getIKSolverName() {return "JacobianPseudoInverse";}
	JacobianPseudoInverse(int maxTries = 50, double targetThreshold = 0.01, double stepweight= 0.5, double dampling = 0.1): IKSolver(maxTries, targetThreshold, stepweight),
		m_dampling(dampling){}

	~JacobianPseudoInverse(void);
	virtual void solveOneStep(Skeleton* chain, std::vector<Vector3_>& targets) override;
};

