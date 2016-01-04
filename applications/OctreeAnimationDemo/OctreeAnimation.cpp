#include "OctreeAnimation.h"
#define USING_TBB
#ifdef USING_TBB
#include "tbb/tbb.h"
#endif

void addition(double& a, double& b, double c)
{
	a += b * c;
}

void add(std::vector<double>& inout, std::vector<double>& addV, double weight = 1)
{
#ifdef USING_TBB
	tbb::parallel_for(size_t(0), size_t(inout.size()), [&] (size_t i) { inout[i] += addV[i] * weight; });
#else
	for(int i = 0; i < inout.size(); ++i)
	{
		inout[i] += addV[i] * weight;
	}
#endif

}

void mul(std::vector<double>& inout, double v)
{
#ifdef USING_TBB
	tbb::parallel_for(size_t(0), size_t(inout.size()), [&] (size_t i) { inout[i] *= v; });
#else
	for(int i = 0; i < inout.size(); ++i)
	{
		inout[i] *= v;
	}
#endif
}

void subdivide(std::vector<double>& inout, double v)
{
#ifdef USING_TBB
	tbb::parallel_for(size_t(0), size_t(inout.size()), [&] (size_t i) { inout[i] /= v; });
#else
	for(int i = 0; i < inout.size(); ++i)
	{
		inout[i] /= v;
	}
#endif
}

void filterring(std::vector<std::vector<double>>& input, std::vector<std::vector<double>>& output, int halfwindowsSize = 10)
{
	std::vector<std::vector<double>> E(input.size());
	output = input;
	E = input;
	if(input.size() <= 2)
	{

	}else
	{

#ifdef USING_TBB
		int nb_elements = input[0].size();
		tbb::parallel_for(size_t(0), size_t(input.size()), [&] (size_t i) { 
			double nb = 1;
			//mul(output[i], weight);
			for(int w = - halfwindowsSize; w <= halfwindowsSize; ++w)
			{
				if(i + w >= 0 && i + w <= input.size() - 1 && ( w != 0))
				{
					double weight = 1;
					nb += weight;
					add(output[i],  input[i + w], weight);
				}
			}
			subdivide(output[i], nb); });
#else
		int nb_elements = input[0].size();
		for(int i = 0; i < input.size(); ++i)
		{
			double nb = 1;
			for(int w = - halfwindowsSize; w <= halfwindowsSize; ++w)
			{
				if(i + w >= 0 && i + w <= input.size() - 1 && ( w != 0))
				{
					double weight = 1;
					nb += weight;
					add(output[i],  input[i + w], weight);
				}
			}
			subdivide(output[i], nb);
		}
#endif
	}
}


void filterring2(std::vector<std::vector<double>>& input, std::vector<std::vector<double>>& output, int halfwindowsSize = 2)
{
	output = input;
	if(input.size() <= 2)
	{

	}else
	{

#ifdef USING_TBB
		int nb_elements = input[0].size();
		tbb::parallel_for(size_t(0), size_t(input.size()), [&] (size_t i) { double nb = 1;
		for(int w = - halfwindowsSize; w <= halfwindowsSize; ++w)
		{
			if(i + w >= 0 && i + w <= input.size() - 1 && ( w != 0))
			{
				double weight = 1;
				nb += weight;
				add(output[i],  input[i + w], weight);
			}
		}
		subdivide(output[i], nb); });
#else
		int nb_elements = input[0].size();
		for(int i = 0; i < input.size(); ++i)
		{
			double nb = 1;
			for(int w = - halfwindowsSize; w <= halfwindowsSize; ++w)
			{
				if(i + w >= 0 && i + w <= input.size() - 1 && ( w != 0))
				{
					double weight = (1 - abs(w / (halfwindowsSize + 1)))* 0.5;
					nb += weight;
					add(output[i],  input[i + w], weight);
				}
			}
			subdivide(output[i], nb);
		}
#endif
	}
}


OctreeAnimation::OctreeAnimation(void)
{
	m_chain.loadFromFile("simple.sk");
	solver = new Etoile::JacobianDLSSolver();
}


OctreeAnimation::~OctreeAnimation(void)
{
}

void OctreeAnimation::addBVH(const std::string& filename)
{
	BVH* bvh = new BVH();
	bvh->loadFromBVHFile(filename);
	bvh->changeOrderToZYX();
	bvh->saveToBVHFile(bvh->getFilePath()+"zyx_");
	this->addBVH(bvh);
}

void OctreeAnimation::addBVH(BVH* bvh)
{
	//BVH TO SEQUENCE
	Sequence* sq = new Sequence();
	sq->m_filename = bvh->getFilePath();
	for(unsigned int i = 0; i < bvh->m_joints.size(); ++i)
	{
		BVH::Joint* current = bvh->m_joints[i];
		std::string name = current->m_name;
		sq->m_indexes[name] = sq->m_headers.size();
		sq->m_headers.push_back(name);
	}

	for(unsigned int j = 0; j < bvh->m_frames.size(); ++j)
	{
		Frame frame = bvh->m_frames[j];
		sq->m_values_frames.push_back(std::vector<Vector3_>());
		std::vector<Vector3_>& sqframe = sq->m_values_frames.back();
		for(unsigned int i = 0; i < bvh->m_joints.size(); ++i)
		{
			BVH::Joint* current = bvh->m_joints[i];
			Vector3_ value;
			for(int h = 0; h < current->m_dof; ++h)
			{
				if(current->m_dims[h].m_name== "Zrotation")
				{
					value[0] = frame.m_values[current->m_dims[h].m_index] * 3.14159265 / 180.0;
				}
				else if(current->m_dims[h].m_name== "Yrotation")
				{
					value[1] = frame.m_values[current->m_dims[h].m_index] * 3.14159265 / 180.0;
				}
				else if(current->m_dims[h].m_name== "Xrotation")
				{
					value[2] = frame.m_values[current->m_dims[h].m_index] * 3.14159265 / 180.0;
				}
			}
			sqframe.push_back(value);
		}
	}

	// add SQ into Data
	this->addSequence(sq);
}

void OctreeAnimation::addSequence(Sequence* sq)
{
	m_usedSequences[sq->m_filename] = sq;
	for(unsigned int i = 0; i < sq->m_values_frames.size(); ++i)
	{
		std::vector<Vector3_>& frame = sq->m_values_frames[i];
		OctreePoint point;
		point.m_current = i;
		//point.m_previous = i - 1;
		point.m_sequence = sq->m_filename;
		//point.m_data.m_headers = sq->m_headers;
		//point.m_data.m_values = frame;

		//point.m_data.m_lambda_values;
		//point.m_position;
		if(i <= 0)
		{
			using namespace Etoile;
			for(unsigned int j = 0; j < m_chain.m_joints.size(); ++j)
			{
				IKChain::Joint* joint  = m_chain.m_joints[j];
				if(sq->m_indexes.find(joint->m_name) != sq->m_indexes.end())
				{
					int idx = sq->m_indexes[joint->m_name];
					point.m_data.m_headers.push_back(joint->m_name);
					m_chain.m_dim_values[joint->m_dims[0].m_idx] = frame[idx][0];
					m_chain.m_dim_values[joint->m_dims[1].m_idx] = frame[idx][1];
					m_chain.m_dim_values[joint->m_dims[2].m_idx] = frame[idx][2];
				}
			}
			m_chain.updateAllDims();
			point.m_data.m_values = m_chain.m_dim_values;
			point.m_position = m_chain.m_dim_globalPositions.back();
		}

		if(i > 0)
		{
			using namespace Etoile;

			//COMPUTE PREFRAME
			std::vector<Vector3_>& preframe = sq->m_values_frames[i-1];
			for(unsigned int j = 0; j < m_chain.m_joints.size(); ++j)
			{
				IKChain::Joint* joint  = m_chain.m_joints[j];
				if(sq->m_indexes.find(joint->m_name) != sq->m_indexes.end())
				{
					int idx = sq->m_indexes[joint->m_name];
					m_chain.m_dim_values[joint->m_dims[0].m_idx] = preframe[idx][0];
					m_chain.m_dim_values[joint->m_dims[1].m_idx] = preframe[idx][1];
					m_chain.m_dim_values[joint->m_dims[2].m_idx] = preframe[idx][2];
				}
			}
			m_chain.updateAllDims();
			Vector3_& endpos = m_chain.m_dim_globalPositions.back();
			std::vector<double> pref = m_chain.m_dim_values;

			//COMPUTE CURRRENTFRAME
			for(unsigned int j = 0; j < m_chain.m_joints.size(); ++j)
			{
				IKChain::Joint* joint  = m_chain.m_joints[j];
				if(sq->m_indexes.find(joint->m_name) != sq->m_indexes.end())
				{
					int idx = sq->m_indexes[joint->m_name];
					point.m_data.m_headers.push_back(joint->m_name);

					m_chain.m_dim_values[joint->m_dims[0].m_idx] = frame[idx][0];
					m_chain.m_dim_values[joint->m_dims[1].m_idx] = frame[idx][1];
					m_chain.m_dim_values[joint->m_dims[2].m_idx] = frame[idx][2];
				}
			}
			m_chain.updateAllDims();
			point.m_data.m_values = m_chain.m_dim_values;
			point.m_position = m_chain.m_dim_globalPositions.back();

			//COMPUTE DELTA THETA
			MatrixX_ jacobian(3, m_chain.m_dims.size());
			VectorX_ dTheta(m_chain.m_dims.size());
			for(unsigned int j = 0; j < m_chain.m_dims.size(); ++j)
			{
				dTheta(j) = point.m_data.m_values[j] - pref[j];
			}


			Vector3_ target(point.m_position);
			Vector3_ distance = (target-endpos);

			for(unsigned int j = 0; j < m_chain.m_dims.size(); ++j)
			{
				IKChain::Dim* dim = m_chain.m_dims[j];
				Vector3_& jointPos = m_chain.m_dim_globalPositions[dim->m_idx];
				Vector3_ boneVector = endpos - jointPos;

				Vector3_ axis = m_chain.m_dim_axis[dim->m_idx];
				int lastDim = dim->m_lastIdx;
				if(lastDim >= 0)
				{
					axis = m_chain.m_dim_globalOrientations[lastDim] * axis;
				}
				Vector3_ axisXYZgradient = axis.cross(boneVector);
				jacobian(0, j) = /*clamp(0 == axisXYZgradient(0)? 0.000001:*/ axisXYZgradient(0)/*, chain.m_dedr_min[j][0], chain.m_dedr_max[j][0])*/;// * m_stepweight;
				jacobian(1, j) =/* clamp(0 == axisXYZgradient(1)? 0.000001:*/ axisXYZgradient(1)/*, chain.m_dedr_min[j][1], chain.m_dedr_max[j][1])*/;// * m_stepweight;
				jacobian(2, j) =/* clamp(0 == axisXYZgradient(2)? 0.000001:*/ axisXYZgradient(2)/*, chain.m_dedr_min[j][2], chain.m_dedr_max[j][2])*/;// * m_stepweight;
			}
			MatrixX_ jacobianTranspose = jacobian.transpose();
			VectorX_ rightside = jacobianTranspose * (distance - jacobian * dTheta);//jacobianTranspose * distance - jacobianTranspose * jacobian * dTheta;

			point.m_data.m_lambda_values.resize(point.m_data.m_values.size());
			for(unsigned int j = 0; j < m_chain.m_dims.size(); ++j)
			{
				dTheta(j) = abs(dTheta(j)) > 0 ? dTheta(j) : 0.00000001;
				double v = abs(rightside(j) / dTheta(j));
				//TODO: makesure has _lambda value not zero
				if(v == 0) v = 100;
				point.m_data.m_lambda_values[j] = v;
			}
		}

		m_octree.insertPoint(point);
	}
	m_octree.updateParameters();

	std::vector<Vector3_> p3;
	p3.push_back(Vector3_(-0.1, 0.2, 0.4));
	this->solve(p3);
}


void OctreeAnimation::updateOctree()
{

}

void OctreeAnimation::saveOctree(const std::string& filename)
{
	saveTXT(filename);
}

void OctreeAnimation::loadOctree(const std::string& filename)
{
	loadTXT(filename);
}

void OctreeAnimation::saveMyOctree(const std::string& filename)
{
	m_octree.writeIntoTXT(filename);
}


std::vector<double> initV;
void OctreeAnimation::solveOriginalTrajectory(int start, int end, const std::string& bvhfile)
{
	BVH bvh;

	if(!bvh.loadFromBVHFile(bvhfile))
	{
		//bvh = m_bvh;
		std::cout<<"no bvh"<<std::endl;
	}

	if(start < 0 || end >= bvh.m_frames.size())
	{
		std::cout<<"start end error : "<<start <<" " <<end <<" max :" << bvh.m_frames.size()<<std::endl;
	}
	std::vector<Frame> temp = bvh.m_frames;
	std::vector<Frame> fs;
	std::vector<Vector3_> points;
	m_chain.reset();
	initV.resize(m_chain.m_dim_values.size());
	for(int i = start ; i < (end < temp.size() ? end : temp.size() - 1); ++i)
	{

		for(int j = 0; j < m_chain.m_joints.size() - 1;++j)
		{
			Etoile::IKChain::Joint* jo =  m_chain.m_joints[j];
			BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
			for(int h = 0; h < 3; ++h)
			{
				jointbvh->m_dims[h].m_value = bvh.m_frames[i].m_values[jointbvh->m_dims[h].m_index];
				m_chain.m_dim_values[jo->m_dims[h].m_idx] = bvh.m_frames[i].m_values[jointbvh->m_dims[h].m_index] * 3.14159265 / 180.0;
			}
		}
		m_chain.updateAllDims();
		if(i == start)
		{
			initV = m_chain.m_dim_values;
		}
		Vector3_ p = m_chain.m_dim_globalPositions.back();
		points.push_back(Vector3_(p[0],p[1],p[2]));
		Frame frame = bvh.createFrame(i);
		Frame frame2 = bvh.m_frames[i];
		fs.push_back(frame);
	}
	bvh.m_frames = fs;
	std::stringstream s;
	s<<bvh.getFilePath()<<"_original_"<<"BVH.bvh";
	bvh.saveToBVHFile(s.str());
	bvh.m_frames = temp;

	/*m_ikchain.reset();
	m_ikchain.m_dim_values = initV;
	solveTrajectory(points, 0, bvh);
	m_ikchain.reset();
	m_ikchain.m_dim_values = initV;
	solveTrajectory(points, 1, bvh);
	m_ikchain.reset();
	m_ikchain.m_dim_values = initV;
	solveTrajectory(points, 2, bvh);
	m_ikchain.reset();
	m_ikchain.m_dim_values = initV;
	solveTrajectory(points, 3, bvh);
	m_ikchain.reset();
	m_ikchain.m_dim_values = initV;
	solveTrajectory(points, 4, bvh);*/
	m_chain.reset();
	m_chain.m_dim_values = initV;
	solve(points, bvh);
	/*m_ikchain.reset();
	m_ikchain.m_dim_values = initV;
	solveTrajectory(points, 6);
	solveTrajectory(points, 6);
	solveTrajectory(points, 7);
	solveTrajectory(points, 8);*/
}

int level = 3;
double epislon = 0.01;


void OctreeAnimation::solve(const std::vector<Vector3_>& points, BVH& bvh)
{
	int solvable = 0;
	int solvable2 = 0;
	std::vector<std::vector<double>> f_values(points.size());
	std::vector<std::vector<double>> f_values_filtered(points.size());
#ifdef USING_TBB
	std::vector<Etoile::IKChain> ikchains;
	for(int i = 0; i < points.size(); ++i)
	{
		ikchains.push_back(m_chain);
	}
	tbb::parallel_for(size_t(0), size_t(points.size()), [&] (size_t i) {
		const Vector3_& point = points[i];
		std::vector<OctreeCell*> treecells = m_octree.p_rootcell->getSubTreeCellsWithPointAndDepth(point, 10);

		if(!m_octree.p_rootcell->isInside(point))
		{
			std::cout<<"the point is not inside the octree: index "<<i <<" : "<<point<<std::endl;
			for(int j = 0; j < ikchains[i].m_dims.size();++j)
			{
				ikchains[i].m_dim_anglelimites[j] = ikchains[i].m_dim_anglelimites_default[j];
				ikchains[i].m_average_values[j] = m_octree.p_rootcell->m_avg[j];
				ikchains[i].m_posture_variation[j] = m_octree.p_rootcell->m_lambda[j];
			}
		}
		else
		{
			int n = level;
			while(n >= treecells.size())
			{
				--n;
			}
			if(n==0)
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon - 0.5, treecells[n]->m_max[j] + epislon + 0.5);
					ikchains[i].m_average_values[j] = treecells[n]->m_avg[j];
					ikchains[i].m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
			else
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon, treecells[n]->m_max[j] + epislon);
					ikchains[i].m_average_values[j] = treecells[n]->m_avg[j];
					ikchains[i].m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
		}

		ikchains[i].m_dim_values = ikchains[i].m_average_values;
		bool sol = true;
		sol = solver->solve(&ikchains[i], point, true);
		if(sol == true)
		{
			solvable += 1; 
		}
		f_values[i] = (ikchains[i].m_dim_values);

	});
#else
	for(int i = 0; i < points.size(); ++i)
	{
		const Vector3_& point = points[i];
		std::vector<OctreeCell*> treecells = m_octree.p_rootcell->getSubTreeCellsWithPointAndDepth(point, 10);

		if(!m_octree.p_rootcell->isInside(point))
		{
			std::cout<<"the point is not inside the octree: index "<<i <<" : "<<point<<std::endl;
			for(int j = 0; j < m_chain.m_dims.size();++j)
			{
				m_chain.m_dim_anglelimites[j] = m_chain.m_dim_anglelimites_default[j];
				m_chain.m_average_values[j] = m_octree.p_rootcell->m_avg[j];
				m_chain.m_posture_variation[j] = m_octree.p_rootcell->m_lambda[j];
			}
		}
		else
		{
			int n = level;
			while(n >= treecells.size())
			{
				--n;
			}
			if(n==0)
			{
				for(int j = 0; j < m_chain.m_dims.size();++j)
				{
					m_chain.m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon - 0.5, treecells[n]->m_max[j] + epislon + 0.5);
					m_chain.m_average_values[j] = treecells[n]->m_avg[j];
					m_chain.m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
			else
			{
				for(int j = 0; j < m_chain.m_dims.size();++j)
				{
					m_chain.m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon, treecells[n]->m_max[j] + epislon);
					m_chain.m_average_values[j] = treecells[n]->m_avg[j];
					m_chain.m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
		}

		m_chain.m_dim_values = m_chain.m_average_values;
		bool sol = true;
		sol = solver->solve(&m_chain, point, true);
		if(sol == true)
		{
			solvable += 1; 
		}
		f_values[i] = (m_chain.m_dim_values); 
	}
#endif

std::vector<Frame> fs(points.size());


	for(int i = 0; i < points.size();++i)
	{
		for(int j = 0; j < ikchains[i].m_joints.size() - 1;++j)
		{
			Etoile::IKChain::Joint* jo =  ikchains[i].m_joints[j];
			BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
			for(int h = 0; h < 3; ++h)
			{
				jointbvh->m_dims[h].m_value = ikchains[i].m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
			}
		}
		Frame frame = bvh.createFrame(i);
		fs[i] = (frame); 
	}



	filterring(f_values, f_values_filtered, 20);

	std::vector<Frame> fsf(points.size());
	for(int i = 0; i < points.size();++i)
	{
			 ikchains[i].m_dim_values = f_values_filtered[i];
			 ikchains[i].updateAllDims();
#ifdef COMPUTETRAJECTORY
			FLOG2 << m_ikchain.m_dim_globalPositions.back()(0)<<","<< m_ikchain.m_dim_globalPositions.back()(1)<<","<< m_ikchain.m_dim_globalPositions.back()(2)<<",";
#endif
			for(int j = 0; j < ikchains[i].m_joints.size() - 1;++j)
			{
				Etoile::IKChain::Joint* jo =  ikchains[i].m_joints[j];
				BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
				for(int h = 0; h < 3; ++h)
				{
					jointbvh->m_dims[h].m_value = ikchains[i].m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
				}
			}
			Frame frame = bvh.createFrame(i);
			fsf[i] = (frame);
	}


	#ifdef USING_TBB
	/*std::vector<Etoile::IKChain> ikchains;
	for(int i = 0; i < points.size(); ++i)
	{
		ikchains.push_back(m_chain);
	}*/
	tbb::parallel_for(size_t(0), size_t(points.size()), [&] (size_t i) {
		const Vector3_& point = points[i];
		std::vector<OctreeCell*> treecells = m_octree.p_rootcell->getSubTreeCellsWithPointAndDepth(point, 10);

		if(!m_octree.p_rootcell->isInside(point))
		{
			std::cout<<"the point is not inside the octree: index "<<i <<" : "<<point<<std::endl;
			for(int j = 0; j < ikchains[i].m_dims.size();++j)
			{
				ikchains[i].m_dim_anglelimites[j] = ikchains[i].m_dim_anglelimites_default[j];
				ikchains[i].m_average_values[j] = m_octree.p_rootcell->m_avg[j];
				ikchains[i].m_posture_variation[j] = m_octree.p_rootcell->m_lambda[j];
			}
		}
		else
		{
			int n = level;
			while(n >= treecells.size())
			{
				--n;
			}
			if(n==0)
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon - 0.5, treecells[n]->m_max[j] + epislon + 0.5);
					ikchains[i].m_average_values[j] = treecells[n]->m_avg[j];
					ikchains[i].m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
			else
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon, treecells[n]->m_max[j] + epislon);
					ikchains[i].m_average_values[j] = treecells[n]->m_avg[j];
					ikchains[i].m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
		}

		ikchains[i].m_dim_values = f_values_filtered[i];
		bool sol = true;
		sol = solver->solve(&ikchains[i], point, true);
		if(sol == true)
		{
			solvable += 1; 
		}
		f_values[i] = (ikchains[i].m_dim_values);

	});
#else
	for(int i = 0; i < points.size(); ++i)
	{
		const Vector3_& point = points[i];
		std::vector<OctreeCell*> treecells = m_octree.p_rootcell->getSubTreeCellsWithPointAndDepth(point, 10);

		if(!m_octree.p_rootcell->isInside(point))
		{
			std::cout<<"the point is not inside the octree: index "<<i <<" : "<<point<<std::endl;
			for(int j = 0; j < m_chain.m_dims.size();++j)
			{
				m_chain.m_dim_anglelimites[j] = m_chain.m_dim_anglelimites_default[j];
				m_chain.m_average_values[j] = m_octree.p_rootcell->m_avg[j];
				m_chain.m_posture_variation[j] = m_octree.p_rootcell->m_lambda[j];
			}
		}
		else
		{
			int n = level;
			while(n >= treecells.size())
			{
				--n;
			}
			if(n==0)
			{
				for(int j = 0; j < m_chain.m_dims.size();++j)
				{
					m_chain.m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon - 0.5, treecells[n]->m_max[j] + epislon + 0.5);
					m_chain.m_average_values[j] = treecells[n]->m_avg[j];
					m_chain.m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
			else
			{
				for(int j = 0; j < m_chain.m_dims.size();++j)
				{
					m_chain.m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon, treecells[n]->m_max[j] + epislon);
					m_chain.m_average_values[j] = treecells[n]->m_avg[j];
					m_chain.m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
		}

		m_chain.m_dim_values = f_values_filtered[i];
		bool sol = true;
		sol = solver->solve(&m_chain, point, true);
		if(sol == true)
		{
			solvable += 1; 
		}
		f_values[i] = (m_chain.m_dim_values); 
	}
#endif

	std::vector<Frame> fs2(points.size());
	for(int i = 0; i < points.size();++i)
	{
		for(int j = 0; j < ikchains[i].m_joints.size() - 1;++j)
		{
			Etoile::IKChain::Joint* jo =  ikchains[i].m_joints[j];
			BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
			for(int h = 0; h < 3; ++h)
			{
				jointbvh->m_dims[h].m_value = ikchains[i].m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
			}
		}
		Frame frame = bvh.createFrame(i);
		fs2[i] = (frame);
	}

	std::vector<Frame> fsf2(points.size());
	filterring2(f_values, f_values_filtered, 3);
	for(int i = 0; i < points.size();++i)
		{
			ikchains[i].m_dim_values = f_values_filtered[i];
			ikchains[i].updateAllDims();
#ifdef COMPUTETRAJECTORY
			FLOG2 << m_ikchain.m_dim_globalPositions.back()(0)<<","<< m_ikchain.m_dim_globalPositions.back()(1)<<","<< m_ikchain.m_dim_globalPositions.back()(2)<<",";
#endif
			for(int j = 0; j < ikchains[i].m_joints.size() - 1;++j)
			{
				Etoile::IKChain::Joint* jo =  ikchains[i].m_joints[j];
				BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
				for(int h = 0; h < 3; ++h)
				{
					jointbvh->m_dims[h].m_value = ikchains[i].m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
				}
			}
			Frame frame = bvh.createFrame(i);
			fsf2[i] = (frame);
		}

	bvh.m_frames = fs;
	std::stringstream s;
	s<<bvh.getFilePath()<<"BVH.bvh";
	bvh.saveToBVHFile(s.str());

	bvh.m_frames = fs2;
	std::stringstream s2;
	s2<<bvh.getFilePath()<<"_"<<"BVH2.bvh";
	bvh.saveToBVHFile(s2.str());

	bvh.m_frames = fsf;
	std::stringstream sf;
	sf<<bvh.getFilePath()<<"BVHf.bvh";
	bvh.saveToBVHFile(sf.str());

	bvh.m_frames = fsf2;
	std::stringstream sf2;
	sf2<<bvh.getFilePath()<<"_"<<"BVHf2.bvh";
	bvh.saveToBVHFile(sf2.str());
}

void OctreeAnimation::solve(const std::vector<Vector3_>& points)
{
	int solvable = 0;
	int solvable2 = 0;
	std::vector<std::vector<double>> f_values(points.size());
	std::vector<std::vector<double>> f_values_filtered(points.size());
#ifdef USING_TBB
	std::vector<Etoile::IKChain> ikchains;
	for(int i = 0; i < points.size(); ++i)
	{
		ikchains.push_back(m_chain);
	}
	tbb::parallel_for(size_t(0), size_t(points.size()), [&] (size_t i) {
		const Vector3_& point = points[i];
		std::vector<OctreeCell*> treecells = m_octree.p_rootcell->getSubTreeCellsWithPointAndDepth(point, 10);

		if(!m_octree.p_rootcell->isInside(point))
		{
			std::cout<<"the point is not inside the octree: index "<<i <<" : "<<point<<std::endl;
			for(int j = 0; j < ikchains[i].m_dims.size();++j)
			{
				ikchains[i].m_dim_anglelimites[j] = ikchains[i].m_dim_anglelimites_default[j];
				ikchains[i].m_average_values[j] = m_octree.p_rootcell->m_avg[j];
				ikchains[i].m_posture_variation[j] = m_octree.p_rootcell->m_lambda[j];
			}
		}
		else
		{
			int n = level;
			while(n >= treecells.size())
			{
				--n;
			}
			if(n==0)
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon - 0.5, treecells[n]->m_max[j] + epislon + 0.5);
					ikchains[i].m_average_values[j] = treecells[n]->m_avg[j];
					ikchains[i].m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
			else
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon, treecells[n]->m_max[j] + epislon);
					ikchains[i].m_average_values[j] = treecells[n]->m_avg[j];
					ikchains[i].m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
		}

		ikchains[i].m_dim_values = ikchains[i].m_average_values;
		bool sol = true;
		sol = solver->solve(&ikchains[i], point, true);
		if(sol == true)
		{
			solvable += 1; 
		}
		f_values[i] = (ikchains[i].m_dim_values);

	});
#else
	for(int i = 0; i < points.size(); ++i)
	{
		const Vector3_& point = points[i];
		std::vector<OctreeCell*> treecells = m_octree.p_rootcell->getSubTreeCellsWithPointAndDepth(point, 10);

		if(!m_octree.p_rootcell->isInside(point))
		{
			std::cout<<"the point is not inside the octree: index "<<i <<" : "<<point<<std::endl;
			for(int j = 0; j < m_chain.m_dims.size();++j)
			{
				m_chain.m_dim_anglelimites[j] = m_chain.m_dim_anglelimites_default[j];
				m_chain.m_average_values[j] = m_octree.p_rootcell->m_avg[j];
				m_chain.m_posture_variation[j] = m_octree.p_rootcell->m_lambda[j];
			}
		}
		else
		{
			int n = level;
			while(n >= treecells.size())
			{
				--n;
			}
			if(n==0)
			{
				for(int j = 0; j < m_chain.m_dims.size();++j)
				{
					m_chain.m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon - 0.5, treecells[n]->m_max[j] + epislon + 0.5);
					m_chain.m_average_values[j] = treecells[n]->m_avg[j];
					m_chain.m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
			else
			{
				for(int j = 0; j < m_chain.m_dims.size();++j)
				{
					m_chain.m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon, treecells[n]->m_max[j] + epislon);
					m_chain.m_average_values[j] = treecells[n]->m_avg[j];
					m_chain.m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
		}

		m_chain.m_dim_values = m_chain.m_average_values;
		bool sol = true;
		sol = solver->solve(&m_chain, point, true);
		if(sol == true)
		{
			solvable += 1; 
		}
		f_values[i] = (m_chain.m_dim_values); 
	}
#endif

	filterring(f_values, f_values_filtered, 20);


#ifdef USING_TBB
	/*std::vector<Etoile::IKChain> ikchains;
	for(int i = 0; i < points.size(); ++i)
	{
		ikchains.push_back(m_chain);
	}*/
	tbb::parallel_for(size_t(0), size_t(points.size()), [&] (size_t i) {
		const Vector3_& point = points[i];
		std::vector<OctreeCell*> treecells = m_octree.p_rootcell->getSubTreeCellsWithPointAndDepth(point, 10);

		if(!m_octree.p_rootcell->isInside(point))
		{
			std::cout<<"the point is not inside the octree: index "<<i <<" : "<<point<<std::endl;
			for(int j = 0; j < ikchains[i].m_dims.size();++j)
			{
				ikchains[i].m_dim_anglelimites[j] = ikchains[i].m_dim_anglelimites_default[j];
				ikchains[i].m_average_values[j] = m_octree.p_rootcell->m_avg[j];
				ikchains[i].m_posture_variation[j] = m_octree.p_rootcell->m_lambda[j];
			}
		}
		else
		{
			int n = level;
			while(n >= treecells.size())
			{
				--n;
			}
			if(n==0)
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon - 0.5, treecells[n]->m_max[j] + epislon + 0.5);
					ikchains[i].m_average_values[j] = treecells[n]->m_avg[j];
					ikchains[i].m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
			else
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon, treecells[n]->m_max[j] + epislon);
					ikchains[i].m_average_values[j] = treecells[n]->m_avg[j];
					ikchains[i].m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
		}

		ikchains[i].m_dim_values = f_values_filtered[i];
		bool sol = true;
		sol = solver->solve(&ikchains[i], point, true);
		if(sol == true)
		{
			solvable += 1; 
		}
		f_values[i] = (ikchains[i].m_dim_values);

	});
#else
	for(int i = 0; i < points.size(); ++i)
	{
		const Vector3_& point = points[i];
		std::vector<OctreeCell*> treecells = m_octree.p_rootcell->getSubTreeCellsWithPointAndDepth(point, 10);

		if(!m_octree.p_rootcell->isInside(point))
		{
			std::cout<<"the point is not inside the octree: index "<<i <<" : "<<point<<std::endl;
			for(int j = 0; j < m_chain.m_dims.size();++j)
			{
				m_chain.m_dim_anglelimites[j] = m_chain.m_dim_anglelimites_default[j];
				m_chain.m_average_values[j] = m_octree.p_rootcell->m_avg[j];
				m_chain.m_posture_variation[j] = m_octree.p_rootcell->m_lambda[j];
			}
		}
		else
		{
			int n = level;
			while(n >= treecells.size())
			{
				--n;
			}
			if(n==0)
			{
				for(int j = 0; j < m_chain.m_dims.size();++j)
				{
					m_chain.m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon - 0.5, treecells[n]->m_max[j] + epislon + 0.5);
					m_chain.m_average_values[j] = treecells[n]->m_avg[j];
					m_chain.m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
			else
			{
				for(int j = 0; j < m_chain.m_dims.size();++j)
				{
					m_chain.m_dim_anglelimites[j] = Vector2_(treecells[n]->m_min[j] - epislon, treecells[n]->m_max[j] + epislon);
					m_chain.m_average_values[j] = treecells[n]->m_avg[j];
					m_chain.m_posture_variation[j] = treecells[n]->m_lambda[j];
				}
			}
		}

		m_chain.m_dim_values = f_values_filtered[i];
		bool sol = true;
		sol = solver->solve(&m_chain, point, true);
		if(sol == true)
		{
			solvable += 1; 
		}
		f_values[i] = (m_chain.m_dim_values); 
	}
#endif

	filterring2(f_values, f_values_filtered, 3);
}