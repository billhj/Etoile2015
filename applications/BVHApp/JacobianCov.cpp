#include "JacobianCov.h"

#define USINGCOV

JacobianCov::~JacobianCov(void)
{

}


void JacobianCov::solveOneStep(Skeleton* chain, std::vector<Vector3_>& targets)
{
	chain->update();
	int tries = 0;
	MatrixX_ jacobian = chain->m_jacobian;
	VectorX_ distance(jacobian.rows());

#ifdef USINGCOV
	/*VectorX_ rotation = VectorX_::Zero(chain->m_dims.size());
	for(int i = 0; i < chain->m_dims.size(); ++i)
	{
		rotation[i] = chain->m_dim_values[i];
	}*/

	if(!m_defined)
	{
		m_mu = VectorX_::Zero(chain->m_dims.size());
		VectorX_ variance = VectorX_::Zero(chain->m_dims.size());
		m_cov = MatrixX_::Identity(chain->m_dims.size(), chain->m_dims.size());
		m_invcov = m_cov.inverse();
		m_defined = true;
	}
	similarIndex(last_state);
#endif

	for(int ei = 0; ei < chain->m_endeffectors.size(); ++ei)
	{
		int endeffectorIdx = chain->m_endeffectors[ei];

		Vector3_& endpos = chain->m_joint_globalPositions[endeffectorIdx];
		Vector3_ dis = (targets[ei]-endpos);
		distance(ei * 3 + 0) = dis(0);
		distance(ei * 3 + 1) = dis(1);
		distance(ei * 3 + 2) = dis(2);
		/*for(unsigned int j = 0; j < 3; ++j)
		{
			Skeleton::Dim& dim = chain->m_dims[j];
			Vector3_ axis = chain->m_dim_axis[dim.m_idx];
			int lastDim = dim.m_lastIdx;
			if(lastDim >= 0)
			{
				axis = chain->m_dim_globalOrientations[lastDim] * axis;
			}
			Vector3_ axisXYZgradient = axis;
			jacobian(ei * 3 + 0, j) = axisXYZgradient(0) * 0.00001;
			jacobian(ei * 3 + 1, j) = axisXYZgradient(1) * 0.00001;
			jacobian(ei * 3 + 2, j) = axisXYZgradient(2) * 0.00001;
		}*/

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
	double dampling2 = m_dampling2 * distance.norm();
	MatrixX_ a =  (2 * jtj  + m_dampling1*lamdaI + dampling2* m_invcov).inverse();
	MatrixX_ b = (2 * jacobianTranspose * distance +  dampling2 * m_invcov * (m_mu - last_state)/ (m_mu - last_state).norm());
	VectorX_ dR = a * b;
#else
	MatrixX_ jacobianTranspose = jacobian.transpose();
	MatrixX_ jtj = jacobian * jacobianTranspose;
	MatrixX_ lamdaI = MatrixX_::Identity(jtj.rows(), jtj.cols());
	VectorX_ dR = jacobianTranspose * ( jtj + lamdaI * m_dampling1).inverse() * distance;
#endif

	/*for(int i = 0; i < 3; ++i)
	{
		chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
		last_state[i] = chain->m_dim_values[i];//dR[i];
	}*/
	/*for(int i = 3; i < 6; ++i)
	{
		last_state[i] = dR[i];
	}
*/
	for(int i = chain->m_startDim4IK; i < chain->m_dims.size(); ++i)
	{
		chain->m_dim_values[i] = castPiRange(chain->m_dim_values[i] + dR[i]);
		last_state[i] = chain->m_dim_values[i];
	}
	chain->update();

}

void trimString( std::string& _string) 
{
	// Trim Both leading and trailing spaces
	size_t start = _string.find_first_not_of(" \t\r\n");
	size_t end   = _string.find_last_not_of(" \t\r\n");

	if(( std::string::npos == start ) || ( std::string::npos == end))
		_string = "";
	else
		_string = _string.substr( start, end-start+1 );
}

bool JacobianCov::loadGaussianFromFile(const std::string& filepath)
{
	std::fstream in( filepath.c_str(), std::ios_base::in );

	if (!in.is_open() || !in.good())
	{
		std::cerr << "[gaussian] : cannot not open file "
			<< filepath
			<< std::endl;
		return false;
	}
#if defined(_DEBUG) || defined(DEBUG)
	std::cout<< "[gaussian] : start loading : "<< filepath <<std::endl;
#endif


	std::string line;
	while( in && !in.eof() )
	{
		//lineIndex++;
		std::getline(in,line);
		if ( in.bad() ){
			std::cout << "  Warning! Could not read file properly! BVH part: skeleton"<<std::endl;
		}

		trimString(line);
		if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) 
		{
			continue;
		}

		int index;
		int dim;
		std::stringstream stream(line);
		stream >> index;
		if(!stream.fail())
		{
			stream >> dim;
			TargetGaussian tg;
			m_gaussians.push_back(tg);
			TargetGaussian& tgref = m_gaussians.back();
			tgref.m_mu = VectorX_::Zero(dim);
			tgref.m_cov = MatrixX_::Zero(dim, dim);

			double value = 0;
			std::getline(in,line);
			std::stringstream stream2(line);
			for(unsigned int n = 0; n < dim; ++n)
			{
				stream2 >> value;
				tgref.m_mu[n] = value;
			}

			value = 0;
			std::getline(in,line);
			std::stringstream stream3(line);
			for(unsigned int w = 0; w < dim; ++w)
			{
				for(unsigned int h = 0; h < dim; ++h)
				{
					stream3 >> value;
					tgref.m_cov(w, h) = value;
				}
			}
			tgref.m_invcov = (tgref.m_cov + MatrixX_::Identity(dim, dim) * 0.000001).inverse();

		}
	}

	in.close();
#if defined(_DEBUG) || defined(DEBUG)
	std::cout<< "[gaussian] : loading is successful "<<std::endl;
#endif
	return true;
}


int JacobianCov::similarIndex(VectorX_ pos)
{
	/*VectorX_ pos = VectorX_::Zero(rpos.size());
	for(unsigned int i = 6; i < rpos.size(); ++i)
	{
		pos[i] = rpos[i];
	}*/

	int idx = -1;
	double minNorm = 99999999999;
	for(unsigned int i = 0; i < m_gaussians.size(); ++i)
	{
		VectorX_ diff = m_gaussians[i].m_mu - pos;
		diff[2] = 0;
		double current = diff.norm();
		if(current < minNorm)
		{
			idx = i;
			minNorm = current;
		}
	}

	m_invcov = m_gaussians[idx].m_invcov;// MatrixX_::Identity(66,66) * 0.001;
	m_mu = m_gaussians[idx].m_mu;
	std::cout<<"idx:           "<<idx<<std::endl;
	return idx;
}