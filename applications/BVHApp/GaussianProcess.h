#pragma once
#include <vector>
#include <Eigen/Dense>
#include <fstream>
#include "Skeleton.h"

typedef Eigen::MatrixXd MatrixX_;
typedef Eigen::Matrix3d Matrix3_;
typedef Eigen::VectorXd VectorX_;
typedef Eigen::Vector3d Vector3_;
typedef Eigen::Vector4d Vector4_;
typedef Eigen::Vector2d Vector2_;
typedef Eigen::AngleAxisd AngleAxis_;

struct TargetGaussian
{
	MatrixX_ m_cov;
	MatrixX_ m_invcov;
	VectorX_ m_mu;
	VectorX_ targets;
};

class GaussianProcess
{
	MatrixX_ m_k;
	MatrixX_ m_k_inverse;
	std::vector<TargetGaussian> m_gaussians;
	Skeleton* m_sk;
public:
	GaussianProcess(void);
	~GaussianProcess(void);
	bool loadGaussianFromFile(const std::string&);
	void loadConfigFile();
	void setSK(Skeleton* sk);
	void computeGP();
	double kernel(VectorX_ x1, VectorX_ x2)
	{
		return exp( -(x1 - x2).norm());
	}

	void buildK();
	TargetGaussian computeASample(const VectorX_& x);
	


};

