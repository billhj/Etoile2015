/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianPseudoInverseSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianPseudoInverseSolver.h"
#include <ctime>
namespace Etoile
{
	using namespace Eigen;
	bool JacobianPseudoInverseSolver::compute(IKChain* chain, Eigen::Vector3f target, bool enableConstraints)
	{
#if( defined( _DEBUG ) || defined( DEBUG ) )
		clock_t time = clock();
#endif
		int tries = 0;
		int columnDim = chain->m_localRotations.size();
		MatrixXf jacobian(3, columnDim);

		VectorXf values(columnDim);
		VectorXf limitsMin(columnDim);
		VectorXf limitsMax(columnDim);
		for(int i = 0; i < columnDim; ++i)
		{
			values(i) = chain->m_values[i];
			limitsMin(i) = chain->m_anglelimites[i][0];
			limitsMax(i) = chain->m_anglelimites[i][1];
		}

		chain->update();
		Vector3f& endpos = chain->m_globalPositions.back();
		Vector3f distance = (target-endpos);

		float beta = 0.5f;

		while (++tries < m_maxTries &&
			distance.norm() > m_targetThreshold)
		{
			Vector3f dT = distance * beta;
			for(unsigned int i = 0; i <  chain->m_joints.size(); ++i)
			{
				IKChain::Joint* joint = chain->m_joints[i];
				std::vector<IKChain::Dim>& dims = joint->m_dims;
				for(unsigned int j = 0; j < dims.size(); ++j)
				{
					IKChain::Dim& dim = dims[j];
					Vector3f& jointPos = chain->m_globalPositions[dim.m_idx];
					Vector3f boneVector = endpos - jointPos;

					Vector3f axis = chain->m_axis[dim.m_idx];
					int lastDim = dim.m_lastIdx;
					if(lastDim >= 0)
					{
						axis = chain->m_globalOrientations[lastDim] * axis;
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

			MatrixXf j_j = pseudoInverse * jacobian;
			MatrixXf I = MatrixXf::Identity(j_j.rows(), j_j.cols());
			MatrixXf nullspace = I - j_j;
			VectorXf dR = pseudoInverse * dT;

			std::cout<<"dR: "<<dR<<std::endl;

			values+=dR;

			std::cout<<"values: "<<values<<std::endl;

			VectorXf minLim = (limitsMin - values);
			VectorXf maxLim = (limitsMax - values);

			for(unsigned int i = 0; i < columnDim; ++i)
			{
				minLim[i] = castPiRange(minLim[i]);
				maxLim[i] = castPiRange(maxLim[i]);
			}
			MatrixXf nullspaceInverse = nullspace.inverse();
			VectorXf minAlph = nullspaceInverse * (minLim);
			VectorXf maxAlph = nullspaceInverse *(maxLim);
#if( defined( _DEBUG ) || defined( DEBUG ) )
		std::cout<<"minAlph: "<<minAlph<<std::endl;
		std::cout<<"maxAlph: "<<maxAlph<<std::endl;
#endif

			for(unsigned int i = 0; i < columnDim; ++i)
			{
				values[i] = castPiRange(values[i]);
				chain->m_localRotations[i] = AngleAxisf(values[i], chain->m_axis[i]);	
			}

			chain->update();
			endpos = chain->m_globalPositions.back();
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

		for(int i = 0; i < columnDim; ++i)
		{
			 chain->m_values[i] = values(i);
			 chain->m_anglelimites[i][0] = limitsMin(i);
			 chain->m_anglelimites[i][1] = limitsMax(i);
		}
		if (tries == m_maxTries)
		{
			return false;
		}
#if( defined( _DEBUG ) || defined( DEBUG ) )
		std::cout<<"iterations: "<<tries<< "distance: "<<distance.norm()<<std::endl;
#endif
		return true;
	}


	//	MatrixXf JacobianPseudoInverseSolver::computeRotations(Eigen::MatrixXf jacobian, Eigen::Vector3f dT)
	//	{
	//		
	////#if( defined( _DEBUG ) || defined( DEBUG ) )
	////		std::cout<<"a: "<<std::endl<<a<<std::endl;
	////		std::cout<<"aInv: "<<std::endl<<aInv<<std::endl;
	////			std::cout<<"pseudoInverse: "<<std::endl<<pseudoInverse<<std::endl;
	////#endif
	//		return pseudoInverse * dT;
	//	}
}
