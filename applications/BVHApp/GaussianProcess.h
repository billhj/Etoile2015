#pragma once
#include <vector>
#include <Eigen/Dense>
#include <fstream>

typedef Eigen::MatrixXd MatrixX_;
typedef Eigen::Matrix3d Matrix3_;
typedef Eigen::VectorXd VectorX_;
typedef Eigen::Vector3d Vector3_;
typedef Eigen::Vector4d Vector4_;
typedef Eigen::Vector2d Vector2_;
typedef Eigen::AngleAxisd AngleAxis_;

class GaussianProcess
{
	MatrixX_ m_k;
	MatrixX_ m_k_inverse;
	std::vector<VectorX_> m_x;
	MatrixX_ m_y;
public:
	GaussianProcess(void);
	~GaussianProcess(void);

	double kernel(VectorX_ x1, VectorX_ x2)
	{
		return exp( -(x1 - x2).norm());
	}

	void buildK(const std::vector<VectorX_>& x_samples, const std::vector<VectorX_>& y_samples)
	{
		if(y_samples.size() < 1) return;
		int len = y_samples[0].size();
		m_y = MatrixX_::Zero(y_samples.size(), len);
		for(unsigned int i = 0 ; i < y_samples.size(); ++i)
		{
			m_y.row(i) = y_samples[i];
		}
		m_x = x_samples;
		int size = m_x.size();
		m_k = MatrixX_::Zero(size, size);
		for(unsigned int i = 0 ; i < size; ++i)
		{
			for(unsigned int j = 0 ; j < size; ++j)
			{
				m_k(i,j) = kernel(m_x[i], m_x[j]);
			}
		}
		m_k_inverse = m_k.inverse();
	}

	VectorX_ computeASample(const VectorX_& x)
	{
		VectorX_ kt = VectorX_::Zero(1, m_x.size());
		for(unsigned int i = 0 ; i < m_x.size(); ++i)
		{
			kt(i) = kernel(x, m_x[i]);
		}
		return kt * m_k_inverse * m_y;
	}



};

