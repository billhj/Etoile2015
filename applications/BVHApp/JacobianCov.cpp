/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianCov.cpp
* @brief 
* @date 1/2/2011
*/
#include "JacobianCov.h"
#include <ctime>
#define USINGCOV

JacobianCov::~JacobianCov(void)
{

}


void JacobianCov::solveOneStep(Skeleton* chain, const std::vector<Vector3_>& targets)
{
	/*for(int i = 0; i < m_mu.size(); ++i){
		chain->m_dim_values[i] = m_mu(i);
	}*/
	chain->update();
	//return;
	last_state = VectorX_::Zero(chain->m_dim_values.size());

	for(int i = 0; i < 3; ++i)
	{
		last_state[i] = 0;// chain->m_dim_values[i];//dR[i];
	}
	/*for(int i = 3; i < 6; ++i)
	{
		last_state[i] = dR[i];
	}
*/
	for(int i = chain->m_startDim4IK; i < chain->m_dims.size(); ++i)
	{
		last_state[i] = chain->m_dim_values[i];
	}

	//int tries = 0;
	MatrixX_ jacobian = chain->m_jacobian;
	VectorX_ distance(jacobian.rows());

#ifdef USINGCOV
	/*VectorX_ rotation = VectorX_::Zero(chain->m_dims.size());
	for(int i = 0; i < chain->m_dims.size(); ++i)
	{
		rotation[i] = chain->m_dim_values[i];
	}*/

	/*if(!m_defined)
	{
		m_mu = VectorX_::Zero(chain->m_dims.size());
		VectorX_ variance = VectorX_::Zero(chain->m_dims.size());
		m_cov = MatrixX_::Identity(chain->m_dims.size(), chain->m_dims.size());
		m_invcov = m_cov.inverse();
		m_defined = true;
	}*/
	//similarIndex(last_state);
#endif

	for(int ei = 0; ei < chain->m_endeffectors.size(); ++ei)
	{
		int endeffectorIdx = chain->m_endeffectors[ei];

		Vector3_& endpos = chain->m_joint_globalPositions[endeffectorIdx];
		Vector3_ dis = (targets[ei]-endpos);
		distance(ei * 3 + 0) = dis(0);
		distance(ei * 3 + 1) = dis(1);
		distance(ei * 3 + 2) = dis(2);

		if(m_activeRoot)
		{
			for(unsigned int j = 0; j < 3; ++j)
			{
				Skeleton::Dim& dim = chain->m_dims[j];
				Vector3_ axis = chain->m_dim_axis[dim.m_idx];
				int lastDim = dim.m_lastIdx;
				if(lastDim >= 0)
				{
					axis = chain->m_dim_globalOrientations[lastDim] * axis;
				}
				Vector3_ axisXYZgradient = axis;
				jacobian(ei * 3 + 0, j) = axisXYZgradient(0) ;//* 0.1;
				jacobian(ei * 3 + 1, j) = axisXYZgradient(1) ;//* 0.1;
				jacobian(ei * 3 + 2, j) = axisXYZgradient(2) ;//* 0.1;
			}
		}

		for(unsigned int j = chain->m_startDim4IK; j < chain->m_dims.size(); ++j)
		{
			if(chain->m_jacobian(ei * 3 + 0, j) < 0.1) continue;

			Skeleton::Dim& dim = chain->m_dims[j];
			Vector3_& jointPos = chain->m_dim_globalPositions[dim.m_idx];
			Vector3_ boneVector = endpos - jointPos;
			if(boneVector.norm() == 0)
			{
				continue;
			}
			//boneVector.normalize();
			Vector3_ axis = chain->m_dim_axis[dim.m_idx];
			int lastDim = dim.m_lastIdx;
			if(lastDim >= 0)
			{
				axis = chain->m_dim_globalOrientations[lastDim] * axis;
			}
			Vector3_ axisXYZgradient = axis.cross(boneVector);

			jacobian(ei * 3 + 0, j) = axisXYZgradient(0);
			jacobian(ei * 3 + 1, j) = axisXYZgradient(1);
			jacobian(ei * 3 + 2, j) = axisXYZgradient(2);
		}
	}
#ifdef USINGCOV
	MatrixX_ jacobianTranspose = jacobian.transpose();
	MatrixX_  jtj = jacobianTranspose * jacobian;
	MatrixX_ lamdaI = MatrixX_::Identity(jtj.rows(), jtj.cols());

	/*double weighta =  distance.norm();
	weighta = weighta * weighta;
	double weightb = (last_state - m_mu).transpose() * m_invcov * (last_state - m_mu);
	double weight = weighta / weightb;
	std::cout <<"w: "<<weightb << "  "<<weight << std::endl;*/
	//double dampling = m_dampling2;// * weight;
	double dampling = m_dampling2 * pow(distance.norm(), 2);
	MatrixX_ a =  (2 * jtj  + m_dampling1*lamdaI + dampling* m_invcov).inverse();
	MatrixX_ b = (2 * jacobianTranspose * distance +  dampling * m_invcov * (m_mu - last_state) );
	VectorX_ dR = a * b;
	//std::cout<<dR<<std::endl;
#else
	MatrixX_ jacobianTranspose = jacobian.transpose();
	MatrixX_ jtj = jacobian * jacobianTranspose;
	MatrixX_ lamdaI = MatrixX_::Identity(jtj.rows(), jtj.cols());
	VectorX_ dR = jacobianTranspose * ( jtj + lamdaI * m_dampling1).inverse() * distance;
#endif

	if(m_activeRoot)
	{
		for(int i = 0; i < 3; ++i)
		{
			chain->m_dim_values[i] = chain->m_dim_values[i] + dR[i];
		}
	}

	/*for(int i = 3; i < 6; ++i)
	{
		last_state[i] = dR[i];
	}
*/
	for(int i = chain->m_startDim4IK; i < chain->m_dims.size(); ++i)
	{
		chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
	}
	chain->update();

}



//void JacobianCov::similarIndex(VectorX_ pos)
//{
//	TargetGaussian tg = m_gp.computeASample(pos);
//	m_invcov = tg.m_invcov;// MatrixX_::Identity(66,66) * 0.001;
//	m_mu = tg.m_mu;
//	//clock_t time = clock();
//	//int idx = -1;
//	//if(false){
//	//	
//	//	double minNorm = 99999999999;
//	//	for(unsigned int i = 0; i < m_gaussians.size(); ++i)
//	//	{
//	//		VectorX_ diff = m_gaussians[i].m_mu - pos;
//	//		diff[2] = 0;
//	//		double current = diff.norm();
//	//		if(current < minNorm)
//	//		{
//	//			idx = i;
//	//			minNorm = current;
//	//		}
//	//	}
//
//	//	m_invcov = m_gaussians[idx].m_invcov;// MatrixX_::Identity(66,66) * 0.001;
//	//	m_mu = m_gaussians[idx].m_mu;
//	//}else
//	//{
//	//	m_mu = pos;
//	//	//m_invcov = m_gp.computeASample(pos);
//	//	idx = 0;
//	//}
//
//	//time = clock() - time;
//	//int ms = double(time) / CLOCKS_PER_SEC * 1000;
//	//std::cout<<"timee elapsed for 10 cluster: "<<ms<<std::endl;
//}


