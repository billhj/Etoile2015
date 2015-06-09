/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianDLSSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianDLSSolver.h"
#include <ctime>

namespace Etoile
{

	using namespace Eigen;
	bool JacobianDLSSolver::solve(Eigen::Vector3f target, bool enableConstraints)
	{
#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif
		int tries = 0;
		int columnDim = p_chain->m_localRotations.size();
		MatrixXf jacobian(3, columnDim);
		p_chain->update();
		Vector3f& endpos = p_chain->m_globalPositions.back();
		Vector3f distance = (target-endpos);

		float beta = 0.5f;

		while (++tries < m_maxTries &&
			distance.norm() > m_targetThreshold)
		{
			Vector3f dT = distance * beta;
			for(unsigned int i = 0; i <  p_chain->m_joints.size(); ++i)
			{
				IKChain::Joint* joint = p_chain->m_joints[i];
				std::vector<IKChain::Dim>& dims = joint->m_dims;
				for(unsigned int j = 0; j < dims.size(); ++j)
				{
					IKChain::Dim& dim = dims[j];
					Vector3f& jointPos = p_chain->m_globalPositions[dim.m_idx];
					Vector3f boneVector = endpos - jointPos;

					Vector3f axis = p_chain->m_axis[dim.m_idx];
					int lastDim = dim.m_lastIdx;
					if(lastDim >= 0)
					{
						axis = p_chain->m_globalOrientations[lastDim] * axis;
					}
					Vector3f axisXYZgradient = axis.cross(boneVector);
					jacobian(0, dim.m_idx) = 0 == axisXYZgradient(0)? 0.000001: axisXYZgradient(0);// * m_stepweight;
					jacobian(1, dim.m_idx) = 0 == axisXYZgradient(1)? 0.000001: axisXYZgradient(1);// * m_stepweight;
					jacobian(2, dim.m_idx) = 0 == axisXYZgradient(2)? 0.000001: axisXYZgradient(2);// * m_stepweight;
				}
			}
			//#if( defined( _DEBUG ) || defined( DEBUG ) )
			//			std::cout<<"jacobian: "<<std::endl<<jacobian<<std::endl;
			//#endif


			MatrixXf jacobianTranspose = jacobian.transpose();
			MatrixXf a =  jacobian * jacobianTranspose;

			MatrixXf aInv = a.inverse();
			MatrixXf pseudoInverse = jacobianTranspose * aInv;
			VectorXf dR = pseudoInverse * dT;


//			float lamda = 1;
//			//compute Lamda
//			{
//				Vector3f JTheta = jacobian * dR - dT;
//				float v = JTheta.transpose() * JTheta;
//				float v2 = dR.transpose() * dR;
//				lamda = v / v2;
//#if( defined( _DEBUG ) || defined( DEBUG ) )
//				//std::cout<<"lamda: "<<lamda<<std::endl;
//#endif
//			}
			MatrixXf dls = jacobianTranspose * ( a +  m_dampling * m_dampling * MatrixXf::Identity(a.rows(), a.cols())).inverse();
			dR = dls * dT;

			for(unsigned int i = 0; i < columnDim; ++i)
			{
				p_chain->m_values[i] = castPiRange(p_chain->m_values[i] + dR[i]);
				p_chain->m_values[i] = clamp(p_chain->m_values[i], p_chain->m_anglelimites[i][0], p_chain->m_anglelimites[i][1]);
				p_chain->m_localRotations[i] = AngleAxisf(p_chain->m_values[i], p_chain->m_axis[i]);	
			}

			p_chain->update();
			endpos = p_chain->m_globalPositions.back();
			distance = (target - endpos);
#if( defined( _DEBUG ) || defined( DEBUG ) )
			//std::cout<<"endpos: "<<endpos.transpose()<<"     distance:  " << distance.norm()<<std::endl;
#endif
		}
#if( defined( _DEBUG ) || defined( DEBUG ) )
		time = clock() - time;
		int ms = double(time) / CLOCKS_PER_SEC * 1000;
		std::cout<<"timee elapsed: "<<ms<<std::endl;
#endif
		if (tries == m_maxTries)
		{
			return false;
		}
#if( defined( _DEBUG ) || defined( DEBUG ) )
		std::cout<<"iterations: "<<tries<< "distance: "<<distance.norm()<<std::endl;
#endif
		return true;
	}


}
