#include "GaussianProcess.h"
#include "JacobianCov.h"

GaussianProcess::GaussianProcess(void)
{
	loadConfigFile();
}


GaussianProcess::~GaussianProcess(void)
{
}

void GaussianProcess::setSK(Skeleton* sk)
{
	m_sk = sk;
	computeGP();
}

void GaussianProcess::computeGP()
{
	for(unsigned int i = 0; i < m_gaussians.size(); ++i)
	{
		for(unsigned int j = 0; j < m_gaussians[i].m_mu.size(); ++j)
		{
			m_sk->m_dim_values[j] = m_gaussians[i].m_mu(j);
		}
		m_sk->update();

		m_gaussians[i].targets = VectorX_::Zero(m_sk->m_endeffectors.size() * 3);
		std::vector<Vector3_> target;
		for(unsigned int j = 0; j < m_sk->m_endeffectors.size(); ++j)
		{
			int idx = m_sk->m_endeffectors[j];
			target.push_back(m_sk->m_joint_globalPositions[idx]);
			m_gaussians[i].targets(j * 3) = target[j](0);
			m_gaussians[i].targets(j * 3 + 1) = target[j](1);
			m_gaussians[i].targets(j * 3 + 2) = target[j](2);
		}

	}
	buildK();
}

void GaussianProcess::buildK()
{
	int size = m_gaussians.size();
	m_k = MatrixX_::Zero(size, size);
	for(unsigned int i = 0 ; i < size; ++i)
	{
		for(unsigned int j = 0 ; j < size; ++j)
		{
			m_k(i,j) = kernel(m_gaussians[i].targets, m_gaussians[j].targets);
		}
	}
	m_k_inverse = m_k.inverse();
}

TargetGaussian GaussianProcess::computeASample(const VectorX_& target)
{
	TargetGaussian tg;
	VectorX_ kt = VectorX_::Zero(m_gaussians.size(), 1);
	for(unsigned int i = 0 ; i < m_gaussians.size(); ++i)
	{
		kt(i) = kernel(target, m_gaussians[i].targets);
	}
	VectorX_ v = kt.transpose() * m_k_inverse;
	tg.m_invcov = MatrixX_::Zero(m_gaussians[0].m_invcov.rows(), m_gaussians[0].m_invcov.cols());
	tg.m_mu = VectorX_::Zero(m_gaussians[0].m_mu.size());
	for(unsigned int i = 0 ; i < v.size(); ++i)
	{
		tg.m_invcov = tg.m_invcov + v(i) * m_gaussians[i].m_invcov;
		tg.m_mu = tg.m_mu + v(i) * m_gaussians[i].m_mu;
	}
	return tg;
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

void GaussianProcess::loadConfigFile()
{
	std::fstream in("jacobiancov.conf", std::ios_base::in );
	if (!in.is_open() || !in.good())
	{
		std::cerr << "[jacobiancov] : cannot not open file "
			<< std::endl;
		return;
	}
#if defined(_DEBUG) || defined(DEBUG)
	std::cout<< "[jacobiancov] : start loading : " <<std::endl;
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

		std::string file;
		std::stringstream stream(line);
		stream >> file;
		bool bt = loadGaussianFromFile(file);
		//if(bt) break;
	}

	in.close();
#if defined(_DEBUG) || defined(DEBUG)
	std::cout<< "[gaussian] : loading is successful "<<std::endl;
#endif
}

bool GaussianProcess::loadGaussianFromFile(const std::string& filepath)
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
