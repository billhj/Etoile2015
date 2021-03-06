/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file JacobianTransposeSolver.cpp
* @brief 
* @date 1/2/2011
*/

#include "JacobianTransposeSolver.h"

namespace Etoile
{
	void JacobianTransposeSolver::solveOneStep(IKChain* chain, Vector3_ target, bool enableConstraints)
	{
		int tries = 0;
		int columnDim = chain->m_dims.size();
		MatrixX_ jacobian(3, columnDim);
		Vector3_& endpos = chain->m_dim_globalPositions.back();
		Vector3_ distance = (target-endpos) * 0.5;


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
			jacobian(0, dim->m_idx) = axisXYZgradient(0);// * m_stepweight;
			jacobian(1, dim->m_idx) = axisXYZgradient(1);// * m_stepweight;
			jacobian(2, dim->m_idx) = axisXYZgradient(2);// * m_stepweight;
		}

#if( defined( _DEBUG ) || defined( DEBUG ) )
		//std::cout<<"jacobian: "<<jacobian<<std::endl;
#endif
		MatrixX_ jacobianTranspose = jacobian.transpose();
		VectorX_ dR = jacobianTranspose * distance;

		for(int i = 0; i < columnDim; ++i)
		{
			chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
			chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
		}

		chain->updateAllDims();
	}

	/**
	**  muli jacobian solver
	*/
	void JacobianTransposeSolver::solveOneStep(IKTree* chain, std::vector<Vector3_> targets, bool enableConstraints)
	{
		chain->updateAllDims();
		int tries = 0;
		int columnDim = chain->m_dims.size();
		int rowDim = 3;
		
		for(int ei = 0; ei < chain->m_dim_end_effector_index.size(); ++ei)
		{

			MatrixX_ jacobian = MatrixX_::Zero(rowDim, columnDim);
			VectorX_ distance(rowDim);
			int endeffectorIdx = chain->m_dim_end_effector_index[ei];

			Vector3_& endpos = chain->m_dim_globalPositions[endeffectorIdx];
			Vector3_ dis = (targets[ei]-endpos);
			distance(0) = dis(0) * 0.5;
			distance(1) = dis(1) * 0.5;
			distance(2) = dis(2) * 0.5;

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
			VectorX_ dR = jacobianTranspose * distance;
			for(int i = 0; i < columnDim; ++i)
			{
				chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
				chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
			}
			chain->updateAllDims();
		}

	}

	//void JacobianTransposeSolver::solveOneStep(IKTree* chain, std::vector<Vector3_> targets, bool enableConstraints)
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
	//		distance(ei * 3 + 0) = dis(0) * 0.01;
	//		distance(ei * 3 + 1) = dis(1) * 0.01;
	//		distance(ei * 3 + 2) = dis(2) * 0.01;

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
	//				jacobian(ei * 3 + 0, j) = 0;
	//				jacobian(ei * 3 + 1, j) = 0;
	//				jacobian(ei * 3 + 2, j) = 0;
	//			}

	//		}

	//	}

	//	MatrixX_ jacobianTranspose = jacobian.transpose();
	//	VectorX_ dR = jacobianTranspose * distance;

	//	for(int i = 0; i < columnDim; ++i)
	//	{
	//		chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
	//		chain->m_dim_values[i] = clamp(chain->m_dim_values[i], chain->m_dim_anglelimites[i][0], chain->m_dim_anglelimites[i][1]);
	//	}
	//	//std::cout<<chain->m_dim_values[0]<<std::endl;
	//	chain->updateAllDims();
	//}
}
