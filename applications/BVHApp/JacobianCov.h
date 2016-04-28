#pragma once
#include "iksolver.h"

struct TargetGaussian
{
	MatrixX_ m_cov;
	MatrixX_ m_invcov;
	VectorX_ m_mu;
};

class JacobianCov : public IKSolver
{
	
	MatrixX_ m_cov;
	MatrixX_ m_invcov;
	VectorX_ m_mu;
	bool m_defined;
	std::vector<TargetGaussian> m_gaussians;

public:
	double m_dampling1;
	double m_dampling2;
	virtual std::string getIKSolverName() {return "JacobianCov";}
	JacobianCov(int maxTries = 50, double targetThreshold = 0.05, double stepweight= 0.5, double dampling = 0.01): IKSolver(maxTries, targetThreshold, stepweight),
		m_dampling1(dampling),m_dampling2(dampling),m_defined(false){ loadGaussianFromFile("gaussian.txt");}

	~JacobianCov(void);
	bool loadGaussianFromFile(const std::string&);
	void setParameters(MatrixX_ incov, VectorX_ mu)
	{
		m_invcov = incov;
		m_mu = mu;
		m_defined = true;
	}
	virtual void solveOneStep(Skeleton* chain, std::vector<Vector3_>& targets) override;

	int similarIndex(std::vector<double> rpos);
};

