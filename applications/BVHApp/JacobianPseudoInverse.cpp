#include "JacobianPseudoInverse.h"


JacobianPseudoInverse::~JacobianPseudoInverse(void)
{
}


void JacobianPseudoInverse::solveOneStep(Skeleton* chain, std::vector<Vector3_>& targets)
{
	chain->update();
	int tries = 0;
	MatrixX_ jacobian = chain->m_jacobian;
	VectorX_ distance(jacobian.rows());
	for(int ei = 0; ei < chain->m_endeffectors.size(); ++ei)
	{
		int endeffectorIdx = chain->m_endeffectors[ei];

		Vector3_& endpos = chain->m_joint_globalPositions[endeffectorIdx];
		Vector3_ dis = (targets[ei]-endpos);
		distance(ei * 3 + 0) = dis(0);
		distance(ei * 3 + 1) = dis(1);
		distance(ei * 3 + 2) = dis(2);

		for(unsigned int j = 3; j < chain->m_dims.size(); ++j)
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

	MatrixX_ jacobianTranspose = jacobian.transpose();
	MatrixX_ jtj = jacobian * jacobianTranspose;
	MatrixX_ lamdaI = MatrixX_::Identity(jtj.rows(), jtj.cols());
	VectorX_ dR = jacobianTranspose * jtj.inverse() * distance;


	for(int i = 3; i < chain->m_dims.size(); ++i)
	{
		chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
	}
	chain->update();

}