/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianCov.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianCov.h"
#include <math.h>
#include <ctime>
namespace Etoile
{

	void JacobianCov::solveOneStep(IKChain* chain, Vector3_ target, bool enableConstraints)
	{
		chain->updateAllDims();
		int tries = 0;
		int columnDim = chain->m_dims.size();
		MatrixX_ jacobian(3, columnDim);
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos);

		for(unsigned int j = 0; j < chain->m_dims.size(); ++j)
		{
			IKChain::Dim* dim = chain->m_dims[j];
			Vector3_& jointPos = chain->m_dim_globalPositions[dim->m_idx];
			Vector3_ boneVector = endpos - jointPos;

			Vector3_ axis = chain->m_dim_axis[dim->m_idx];
			int lastDim = dim->m_lastIdx;
			if(lastDim >= 0)
			{
				axis = chain->m_dim_globalOrientations[lastDim] * axis;
			}
			Vector3_ axisXYZgradient = axis.cross(boneVector);

			jacobian(0, j) = axisXYZgradient(0);
			jacobian(1, j) = axisXYZgradient(1);
			jacobian(2, j) = axisXYZgradient(2);

		}

		MatrixX_ jacobianTranspose = jacobian.transpose();
#define USINGMYDLS
#ifdef USINGMYDLS

		MatrixX_ jtj =  jacobianTranspose * jacobian;
		MatrixX_ lamdaI = MatrixX_::Zero(jtj.rows(), jtj.cols());
		for(int i = 0; i < jtj.rows(); ++i)
		{
			lamdaI(i,i) = chain->m_posture_variation[i];   //m_dampling / ( pow(10.0, i/3) * +1); 
			assert(lamdaI(i,i) > 0.0000000000000000000001 && lamdaI(i,i) < 99999999999999);
		}

		MatrixX_ a = jtj + lamdaI;
		MatrixX_ b = jacobianTranspose * distance;// + thetax * v2;
		VectorX_ dR = a.inverse() * b;
#else
		MatrixX_ a =  jacobian * jacobianTranspose;

		MatrixX_ dls = jacobianTranspose * ( a +  m_dampling * MatrixX_::Identity(a.rows(), a.cols())).inverse();
		VectorX_ dR = dls * dT;
#endif

		for(int i = 0; i < columnDim; ++i)
		{
			chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
			chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
		}
		//std::cout<<chain->m_dim_values[0]<<std::endl;
		chain->updateAllDims();
	}


	/**
	**  muli jacobian solver
	*/
	void JacobianCov::solveOneStep(IKTree* chain, std::vector<Vector3_> targets, bool enableConstraints)
	{
		chain->updateAllDims();
		int tries = 0;
		int columnDim = chain->m_dims.size();
		int rowDim = 3;
		
		for(int ei = 0; ei < chain->m_dim_end_effector_index.size(); ++ei)
		{

			MatrixX_ jacobian = MatrixX_::Zero(rowDim, columnDim);
			int endeffectorIdx = chain->m_dim_end_effector_index[ei];

			Vector3_& endpos = chain->m_dim_globalPositions[endeffectorIdx];
			Vector3_ distance = (targets[ei]-endpos);

			for(unsigned int j = 0; j < chain->m_dims.size(); ++j)
			{
				IKChain::Dim* dim = chain->m_dims[j];
				Vector3_& jointPos = chain->m_dim_globalPositions[dim->m_idx];
				Vector3_ boneVector = endpos - jointPos;

				Vector3_ axis = chain->m_dim_axis[dim->m_idx];
				int lastDim = dim->m_lastIdx;
				if(lastDim >= 0)
				{
					axis = chain->m_dim_globalOrientations[lastDim] * axis;
				}
				Vector3_ axisXYZgradient = axis.cross(boneVector);
				int identifier = chain->m_dim_identifier[j];
				int eiId = (int)(pow(2.0, ei));
				if((identifier & eiId) == eiId)
				{
					jacobian(0, j) = axisXYZgradient(0);
					jacobian(1, j) = axisXYZgradient(1);
					jacobian(2, j) = axisXYZgradient(2);
				}
				else
				{
					jacobian(0, j) = 0;//0.00000000000000000001;
					jacobian(1, j) = 0;//0.0000000000000000000001;
					jacobian(2, j) = 0;//0.0000000000000000000001;
				}
			}

			MatrixX_ jacobianTranspose = jacobian.transpose();
			MatrixX_ jtj =  jacobianTranspose * jacobian;
			MatrixX_ lamdaI = MatrixX_::Zero(jtj.rows(), jtj.cols());
			for(int i = 0; i < jtj.rows(); ++i)
			{
				lamdaI(i,i) = chain->m_posture_variation[i];   
			}

			MatrixX_ a = jtj + lamdaI;
			MatrixX_ b = jacobianTranspose * distance;
			MatrixX_ aInv = a.inverse();
			VectorX_ dR = aInv * b;
			for(int i = 0; i < columnDim; ++i)
			{
				chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
				chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
			}
			chain->updateAllDims();
		}

	}



	//void JacobianCov::solveOneStep(IKTree* chain, std::vector<Vector3_> targets, bool enableConstraints)
	//{
	//	chain->updateAllDims();
	//	int tries = 0;
	//	int columnDim = chain->m_dims.size();
	//	int rowDim = chain->m_dim_end_effector_index.size() * 3;
	//	MatrixX_ jacobian = MatrixX_::Zero(rowDim, columnDim);
	//	VectorX_ distance(rowDim);
	//	for(int ei = 0; ei < chain->m_dim_end_effector_index.size(); ++ei)
	//	{
	//		int endeffectorIdx = chain->m_dim_end_effector_index[ei];

	//		Vector3_& endpos = chain->m_dim_globalPositions[endeffectorIdx];
	//		Vector3_ dis = (targets[ei]-endpos);
	//		distance(ei * 3 + 0) = dis(0);
	//		distance(ei * 3 + 1) = dis(1);
	//		distance(ei * 3 + 2) = dis(2);

	//		for(unsigned int j = 0; j < chain->m_dims.size(); ++j)
	//		{
	//			IKChain::Dim* dim = chain->m_dims[j];
	//			Vector3_& jointPos = chain->m_dim_globalPositions[dim->m_idx];
	//			Vector3_ boneVector = endpos - jointPos;

	//			Vector3_ axis = chain->m_dim_axis[dim->m_idx];
	//			int lastDim = dim->m_lastIdx;
	//			if(lastDim >= 0)
	//			{
	//				axis = chain->m_dim_globalOrientations[lastDim] * axis;
	//			}
	//			Vector3_ axisXYZgradient = axis.cross(boneVector);
	//			int identifier = chain->m_dim_identifier[j];
	//			if(identifier & (int)(pow(2.0, ei)) == (int)(pow(2.0, ei)))
	//			{
	//				jacobian(ei * 3 + 0, j) = axisXYZgradient(0);
	//				jacobian(ei * 3 + 1, j) = axisXYZgradient(1);
	//				jacobian(ei * 3 + 2, j) = axisXYZgradient(2);
	//			}
	//			else
	//			{
	//				jacobian(ei * 3 + 0, j) = 0;//0.00000000000000000001;
	//				jacobian(ei * 3 + 1, j) = 0;//0.0000000000000000000001;
	//				jacobian(ei * 3 + 2, j) = 0;//0.0000000000000000000001;
	//			}

	//		}

	//	}

	//	MatrixX_ jacobianTranspose = jacobian.transpose();
	//	MatrixX_ jtj =  jacobianTranspose * jacobian;
	//	MatrixX_ lamdaI = MatrixX_::Identity(jtj.rows(), jtj.cols());
	//	MatrixX_ betax = MatrixX_::Zero(jtj.rows(), jtj.cols());
	//	for(int i = 0; i < jtj.rows(); ++i)
	//	{
	//		lamdaI(i,i) = chain->m_posture_variation[i];   
	//	}

	//	MatrixX_ a = jtj + lamdaI;
	//	MatrixX_ b = jacobianTranspose * distance;
	//	MatrixX_ aInv = a.inverse();
	//	VectorX_ dR = aInv * b;
	//	for(int i = 0; i < columnDim; ++i)
	//	{
	//		chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
	//		chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
	//	}
	//	chain->updateAllDims();
	//}
}