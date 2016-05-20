/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file GPIKsolver.h
* @brief 
* @date 1/2/2011
*/
#pragma once
#include "iksolver.h"
class GPIKsolver :
	public IKSolver
{
	std::vector<VectorX_> m_mus;
	std::vector<VectorX_> m_targets;
	VectorX_ m_mu;
	Skeleton* m_sk;
	MatrixX_ m_k;
	MatrixX_ m_k_inverse;
public:
	GPIKsolver(Skeleton* sk);
	~GPIKsolver(void);
	virtual std::string getIKSolverName() {return "GaussianProcess";}
	virtual void solveOneStep(Skeleton* chain, const std::vector<Vector3_>& targets) override;
	void loadConfig()
	{
		loadPoses("poses.txt");
	}
	bool loadPoses(const std::string&  filepath);
	void setSK(Skeleton* sk);
	void computeGP();
	double kernel(VectorX_ x1, VectorX_ x2)
	{
		//return exp( -pow((x1 - x2).norm(), 2));
		return exp( -(x1 - x2).norm());
	}
	void computeASample(const VectorX_& x);
	void buildK();
};

