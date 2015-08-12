#include "OctreeSkeleton.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>
#include "TimeWin32.h"
#include "FLog.h"
#define USING_TBB
#ifdef USING_TBB
#include "tbb/tbb.h"
//using namespace tbb;
#endif

//#define USINGLINEAREQUATION
//#define USINGAV
#define USINGMAX

void trimString( std::string& _string) {
	// Trim Both leading and trailing spaces

	size_t start = _string.find_first_not_of(" \t\r\n");
	size_t end   = _string.find_last_not_of(" \t\r\n");

	if(( std::string::npos == start ) || ( std::string::npos == end))
		_string = "";
	else
		_string = _string.substr( start, end-start+1 );
}

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

#include "geometry/RenderManager.h"
#include "geometry/SceneManager.h"
#include "geometry/Scene.h"

OctreeSkeleton::OctreeSkeleton(void):ObjectRenderer("octreeskeleton")
{
	m_dataIsLoaded = false;

	Etoile::Scene* scene = Etoile::SceneManager::getInstance()->getCurrentScene();

	Etoile::Entity* entity = new Etoile::Entity("octreeskeleton", scene);
	entity->setComponent(Etoile::ComponentType::RENDER_COMPONENT, this);
	Etoile::RenderManager::getInstance()->add(this);

	drawType = 0;
	m_depth = 5;
}

void OctreeSkeleton::reload(const std::string& name)
{
	std::cout<<"start"<<std::endl;
	m_bvh = BVH();
	m_bvh.loadFromBVHFile(name +".bvh");
	m_ikchain = Etoile::IKChain();
	m_ikchain.loadFromFile(name+".sk");
	loadFromCSVFile(name+".csv");

	solver = new Etoile::JacobianDLSSolver();
	m_dataIsLoaded = false;
	//p_tree = new Octree(Vec3(-0.15, 0.2, 0.15), Vec3(0.25, 0.4, 0.45), true);
	computePoints();

	loadDataIntoOctree();
	//computeMinMaxAverage();
	computeMinMaxAverageByDepth(7);
	std::cout<<"end"<<std::endl;
}

OctreeSkeleton::~OctreeSkeleton(void)
{
}

int _start = 0;
void OctreeSkeleton::solveOriginalTrajectory(int start, int end, const std::string& bvhfile)
{
	BVH bvh;
	
	if(!bvh.loadFromBVHFile(bvhfile))
	{
		bvh = m_bvh;
	}

	if(start < 0 || end >= bvh.m_frames.size())
	{
		std::cout<<"start end error : "<<start <<" " <<end <<" max :" << bvh.m_frames.size()<<std::endl;
	}
	std::vector<Frame> temp = bvh.m_frames;
	std::vector<Frame> fs;
	std::vector<Vec3> points;
	_start = start;
	m_ikchain.reset();
	std::vector<double> initV(m_ikchain.m_dim_values.size());
	for(int i = start ; i < (end < temp.size() ? end : temp.size() - 1); ++i)
	{

		for(int j = 0; j < m_ikchain.m_joints.size() - 1;++j)
		{
			Etoile::IKChain::Joint* jo =  m_ikchain.m_joints[j];
			BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
			for(int h = 0; h < 3; ++h)
			{
				jointbvh->m_dims[h].m_value = bvh.m_frames[i].m_values[jointbvh->m_dims[h].m_index];
				m_ikchain.m_dim_values[jo->m_dims[h].m_idx] = bvh.m_frames[i].m_values[jointbvh->m_dims[h].m_index] * 3.14159265 / 180.0;
			}
		}
		m_ikchain.updateAllDims();
		if(i == start)
		{
			initV = m_ikchain.m_dim_values;
		}
		Etoile::Vector3_ p = m_ikchain.m_dim_globalPositions.back();
		points.push_back(Vec3(p[0],p[1],p[2]));
		Frame frame = bvh.createFrame();
		fs.push_back(frame);
	}
	bvh.m_frames = fs;
	std::stringstream s;
	TimeWin32 start2;
	double time2 = start2.getCurrentTime();
	s<<_name<<"_original_"<<"BVH.bvh";
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
	m_ikchain.reset();
	m_ikchain.m_dim_values = initV;
	solveTrajectory(points, 5, bvh);
	/*m_ikchain.reset();
	m_ikchain.m_dim_values = initV;
	solveTrajectory(points, 6);
	solveTrajectory(points, 6);
	solveTrajectory(points, 7);
	solveTrajectory(points, 8);*/
}

void OctreeSkeleton::solveOriginalPrefilterTrajectory(int start, int end, const std::string& bvhfile)
{
	BVH bvh;
	
	if(!bvh.loadFromBVHFile(bvhfile))
	{
		bvh = m_bvh;
	}

	if(start < 0 || end >= bvh.m_frames.size())
	{
		std::cout<<"start end error : "<<start <<" " <<end <<" max :" << bvh.m_frames.size()<<std::endl;
	}
	std::vector<Frame> temp = bvh.m_frames;
	std::vector<Frame> fs;
	std::vector<Vec3> points;
	_start = start;
	m_ikchain.reset();
	std::vector<double> initV;
	for(int i = start ; i < end; ++i)
	{

		for(int j = 0; j < m_ikchain.m_joints.size() - 1;++j)
		{
			Etoile::IKChain::Joint* jo =  m_ikchain.m_joints[j];
			BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
			for(int h = 0; h < 3; ++h)
			{
				jointbvh->m_dims[h].m_value = bvh.m_frames[i].m_values[jointbvh->m_dims[h].m_index];
				m_ikchain.m_dim_values[jo->m_dims[h].m_idx] = bvh.m_frames[i].m_values[jointbvh->m_dims[h].m_index] * 3.14159265 / 180.0;
			}
		}
		m_ikchain.updateAllDims();
		if(i == start)
		{
			initV = m_ikchain.m_dim_values;
		}
		Etoile::Vector3_ p = m_ikchain.m_dim_globalPositions.back();
		points.push_back(Vec3(p[0],p[1],p[2]));
		Frame frame = bvh.createFrame();
		fs.push_back(frame);
	}
	bvh.m_frames = fs;
	std::stringstream s;
	TimeWin32 start2;
	double time2 = start2.getCurrentTime();
	s<<_name<<"_original_"<<"BVH.bvh";
	bvh.saveToBVHFile(s.str());
	bvh.m_frames = temp;

	solvePrefilterTrajectory(points, 0, bvh);
	solvePrefilterTrajectory(points, 1, bvh);
	solvePrefilterTrajectory(points, 2, bvh);
	solvePrefilterTrajectory(points, 3, bvh);
	solvePrefilterTrajectory(points, 4, bvh);
	solvePrefilterTrajectory(points, 5, bvh);
	/*solvePrefilterTrajectory(points, 6);
	solvePrefilterTrajectory(points, 7);
	solvePrefilterTrajectory(points, 8);*/
}

void OctreeSkeleton::solveTrajectory(const std::vector<Vec3>& points, int depth, BVH& bvh)
{
	int solvable = 0;
	int solvable2 = 0;
	FLOG<<points.size()<< "points, depth" <<depth<<std::endl;
	FrameData& iniv = m_framesData[_start];

	std::vector<Frame> temp = bvh.m_frames;
	std::vector<Frame> fs(points.size());
	std::vector<std::vector<double>> f_values(points.size());
	std::vector<std::vector<double>> f_values_filtered(points.size());
	solver->setMaxNumberOfTries(50);

	std::vector<Etoile::IKChain> ikchains;
	for(int i = 0; i < points.size(); ++i)
	{
		ikchains.push_back(m_ikchain);
	}


	clock_t time = clock();
	

#ifdef USING_TBB
	tbb::parallel_for(size_t(0), size_t(points.size()), [&] (size_t i) {Vec3 point = points[i];
			std::vector<Octree*> trees = m_treeowner.p_octreeRoot->getSubTreesWithPointAndDepth(point, depth);
			if(depth != 0)
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Etoile::Vector2_(trees[1]->m_cell_min[j], trees[1]->m_cell_max[j]);
					ikchains[i].m_average_values[j] = /*(tree->m_cell_min[j] + tree->m_cell_max[j])*0.5;*/trees.back()->m_cell_average[j];
					ikchains[i].m_posture_variation[j] = trees.back()->m_lamda[j];
				}
			}

			bool sol = true;
			
			ikchains[i].m_dim_values = ikchains[i].m_average_values;
			if(i == 0)
			{
				ikchains[i].m_dim_values = iniv.m_values;
				
				sol = solver->solve(&ikchains[i], Etoile::Vector3_(point.x, point.y, point.z), true);
			}
			else
			{
				sol = solver->solve(&ikchains[i],Etoile::Vector3_(point.x, point.y, point.z), true);
			}
			if(sol == true)
			{
				solvable += 1; 
			}
			
			f_values[i] = (ikchains[i].m_dim_values);
			for(int j = 0; j < ikchains[i].m_joints.size() - 1;++j)
			{
				Etoile::IKChain::Joint* jo =  ikchains[i].m_joints[j];
				BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
				for(int h = 0; h < 3; ++h)
				{
					jointbvh->m_dims[h].m_value = ikchains[i].m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
				}
			}
			Frame frame = bvh.createFrame();
			fs[i] = (frame); } );
#else
	{
		for(int i = 0; i < points.size();++i)
		{
			Vec3 point = points[i];
			std::vector<Octree*> trees = m_treeowner.p_octreeRoot->getSubTreesWithPointAndDepth(point, depth);
			if(depth != 0)
			{
				for(int j = 0; j < m_ikchain.m_dims.size();++j)
				{
					m_ikchain.m_dim_anglelimites[j] = Etoile::Vector2_(trees[1]->m_cell_min[j], trees[1]->m_cell_max[j]);
					m_ikchain.m_average_values[j] = /*(tree->m_cell_min[j] + tree->m_cell_max[j])*0.5;*/trees.back()->m_cell_average[j];
					m_ikchain.m_posture_variation[j] = trees.back()->m_lamda[j];
				}
			}

			bool sol = true;
			
			m_ikchain.m_dim_values = m_ikchain.m_average_values;
			if(i == 0)
			{
				m_ikchain.m_dim_values = iniv.m_values;
				
				sol = solver->solve(&m_ikchain, Etoile::Vector3_(point.x, point.y, point.z), true);
			}
			else
			{
				sol = solver->solve(&m_ikchain,Etoile::Vector3_(point.x, point.y, point.z), true);
			}
			if(sol == true)
			{
				solvable += 1; 
			}
			
			f_values[i] = (m_ikchain.m_dim_values);
			for(int j = 0; j < m_ikchain.m_joints.size() - 1;++j)
			{
				Etoile::IKChain::Joint* jo =  m_ikchain.m_joints[j];
				BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
				for(int h = 0; h < 3; ++h)
				{
					jointbvh->m_dims[h].m_value = m_ikchain.m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
				}
			}
			Frame frame = bvh.createFrame();
			fs[i] = (frame);
		}
	}
	
#endif
	time = clock() - time;



	std::vector<Frame> fsf(points.size());
	clock_t timef = clock();
	filterring(f_values, f_values_filtered,20);
	timef = clock() - timef;

	{
		for(int i = 0; i < points.size();++i)
		{
			m_ikchain.m_dim_values = f_values_filtered[i];
			for(int j = 0; j < m_ikchain.m_joints.size() - 1;++j)
			{
				Etoile::IKChain::Joint* jo =  m_ikchain.m_joints[j];
				BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
				for(int h = 0; h < 3; ++h)
				{
					jointbvh->m_dims[h].m_value = m_ikchain.m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
				}
			}
			Frame frame = bvh.createFrame();
			fsf[i] = (frame);
		}
	}
	

	solver->setMaxNumberOfTries(50);
	
	clock_t time2 = clock();

	std::vector<Frame> fs2(points.size());
#ifdef USING_TBB
	tbb::parallel_for(size_t(0), size_t(points.size()), [&] (size_t i) {Vec3 point = points[i];
			std::vector<Octree*> trees = m_treeowner.p_octreeRoot->getSubTreesWithPointAndDepth(point, depth);
			if(depth != 0)
			{
				for(int j = 0; j < ikchains[i].m_dims.size();++j)
				{
					ikchains[i].m_dim_anglelimites[j] = Etoile::Vector2_(trees[1]->m_cell_min[j], trees[1]->m_cell_max[j]);
					ikchains[i].m_average_values[j] = /*(tree->m_cell_min[j] + tree->m_cell_max[j])*0.5;*/trees.back()->m_cell_average[j];
					ikchains[i].m_posture_variation[j] = trees.back()->m_lamda[j];
				}
			}

			bool sol = true;
			ikchains[i].m_dim_values = f_values_filtered[i];

			if(i == 0)
			{
				ikchains[i].m_dim_values = iniv.m_values;

				sol = solver->solve(&ikchains[i], Etoile::Vector3_(point.x, point.y, point.z), true);
			}
			else
				sol = solver->solve(&ikchains[i],Etoile::Vector3_(point.x, point.y, point.z), true);
			if(sol == true)
			{
				solvable2 += 1; 
			}
			f_values[i] = (ikchains[i].m_dim_values);

			for(int j = 0; j < ikchains[i].m_joints.size() - 1;++j)
			{
				Etoile::IKChain::Joint* jo =  ikchains[i].m_joints[j];
				BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
				for(int h = 0; h < 3; ++h)
				{
					jointbvh->m_dims[h].m_value = ikchains[i].m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
				}
			}
			Frame frame = bvh.createFrame();
			fs2[i] = (frame); });
#else
	
	{
		for(int i = 0; i < points.size();++i)
		{
			Vec3 point = points[i];
			std::vector<Octree*> trees = m_treeowner.p_octreeRoot->getSubTreesWithPointAndDepth(point, depth);
			if(depth != 0)
			{
				for(int j = 0; j < m_ikchain.m_dims.size();++j)
				{
					m_ikchain.m_dim_anglelimites[j] = Etoile::Vector2_(trees[1]->m_cell_min[j], trees[1]->m_cell_max[j]);
					m_ikchain.m_average_values[j] = /*(tree->m_cell_min[j] + tree->m_cell_max[j])*0.5;*/trees.back()->m_cell_average[j];
					m_ikchain.m_posture_variation[j] = trees.back()->m_lamda[j];
				}
			}

			bool sol = true;
			m_ikchain.m_dim_values = f_values_filtered[i];

			if(i == 0)
			{
				m_ikchain.m_dim_values = iniv.m_values;

				sol = solver->solve(&m_ikchain, Etoile::Vector3_(point.x, point.y, point.z), true);
			}
			else
				sol = solver->solve(&m_ikchain,Etoile::Vector3_(point.x, point.y, point.z), true);
			if(sol == true)
			{
				solvable2 += 1; 
			}
			f_values[i] = (m_ikchain.m_dim_values);

			for(int j = 0; j < m_ikchain.m_joints.size() - 1;++j)
			{
				Etoile::IKChain::Joint* jo =  m_ikchain.m_joints[j];
				BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
				for(int h = 0; h < 3; ++h)
				{
					jointbvh->m_dims[h].m_value = m_ikchain.m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
				}
			}
			Frame frame = bvh.createFrame();
			fs2[i] = (frame);
		}
	}

#endif
	time2 = clock() - time2;





	std::vector<Frame> fsf2(points.size());
	clock_t timef2 = clock();
	filterring(f_values, f_values_filtered, 2);
	timef2 = clock() - timef2;
	{
		for(int i = 0; i < points.size();++i)
		{
			m_ikchain.m_dim_values = f_values_filtered[i];
			for(int j = 0; j < m_ikchain.m_joints.size() - 1;++j)
			{
				Etoile::IKChain::Joint* jo =  m_ikchain.m_joints[j];
				BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
				for(int h = 0; h < 3; ++h)
				{
					jointbvh->m_dims[h].m_value = m_ikchain.m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
				}
			}
			Frame frame = bvh.createFrame();
			fsf2[i] = (frame);
		}
	}


	FLOG<<"SOLVABLE " <<solvable <<" / " <<points.size()<<std::endl;
	std::cout<<"SOLVABLE " <<solvable <<" / " <<points.size()<<std::endl;
	std::cout<<"SOLVABLE2 " <<solvable2 <<" / " <<points.size()<<std::endl;
	int ms = double(time) / CLOCKS_PER_SEC * 1000;
	int ms2 = double(time2) / CLOCKS_PER_SEC * 1000;
	int msf = double(timef) / CLOCKS_PER_SEC * 1000;
	int msf2 = double(timef2) / CLOCKS_PER_SEC * 1000;
	std::cout<<"time "<<ms <<" and "<<ms2<<std::endl;
	std::cout<<"time filter "<<msf<<" and "<<msf2<<std::endl;

	bvh.m_frames = fs;
	std::stringstream s;
	s<<_name+"_depth_"<<depth<<"_"<<"BVH.bvh";
	bvh.saveToBVHFile(s.str());

	bvh.m_frames = fs2;
	std::stringstream s2;
	s2<<_name+"_depth_"<<depth<<"_"<<"BVH2.bvh";
	bvh.saveToBVHFile(s2.str());

	bvh.m_frames = fsf;
	std::stringstream sf;
	sf<<_name+"_depth_"<<depth<<"_"<<"BVHf.bvh";
	bvh.saveToBVHFile(sf.str());

	bvh.m_frames = fsf2;
	std::stringstream sf2;
	sf2<<_name+"_depth_"<<depth<<"_"<<"BVHf2.bvh";
	bvh.saveToBVHFile(sf2.str());

	bvh.m_frames = temp;
}

void OctreeSkeleton::solvePrefilterTrajectory(const std::vector<Vec3>& points, int depth, BVH& bvh)
{
	int solvable = 0;
	FLOG<<points.size()<< "points, depth" <<depth<<std::endl;
	//FrameData& iniv = m_framesData[_start];

	std::vector<Frame> temp = bvh.m_frames;
	TimeWin32 start;
	double time1 = start.getCurrentTime();
	std::vector<Frame> fs;

	std::vector<Octree*> trees;
	for(int i = 0; i < points.size();++i)
	{
		Vec3 point = points[i];
		Octree* tree = m_treeowner.p_octreeRoot->getSubTreeWithPointAndDepth(point, depth);
		trees.push_back(tree);
	}

	for(int i = 0; i < points.size();++i)
	{
		Vec3 point = points[i];
		FLOG<<i<<"   "<<std::endl;
		Octree* tree = trees[i];
		if(depth != 0)
		{
			for(int j = 0; j < m_ikchain.m_dims.size();++j)
			{
				m_ikchain.m_dim_anglelimites[j] = Etoile::Vector2_(tree->p_owner->p_octreeRoot->m_cell_min[j], tree->p_owner->p_octreeRoot->m_cell_max[j]);
				int nb = 1;
				m_ikchain.m_average_values[j] = tree->m_cell_average[j];
				m_ikchain.m_posture_variation[j] = tree->m_lamda[j];
				if(i > 0)
				{
					m_ikchain.m_average_values[j] += trees[i - 1]->m_cell_average[j];
					m_ikchain.m_dim_anglelimites[j] += Etoile::Vector2_(trees[i - 1]->m_cell_min[j], trees[i - 1]->m_cell_max[j]);
					++nb;
				}
				if(i < points.size() - 1)
				{
					m_ikchain.m_average_values[j] += trees[i + 1]->m_cell_average[j];
					m_ikchain.m_dim_anglelimites[j] += Etoile::Vector2_(trees[i + 1]->m_cell_min[j], trees[i + 1]->m_cell_max[j]);
					++nb;
				}
				m_ikchain.m_average_values[j] /= nb;
				m_ikchain.m_dim_anglelimites[j] /= nb;

				m_ikchain.m_dedr_max[j][0] = tree->m_cell_dedr_max[j][0];
				m_ikchain.m_dedr_max[j][1] = tree->m_cell_dedr_max[j][1];
				m_ikchain.m_dedr_max[j][2] = tree->m_cell_dedr_max[j][2];
				m_ikchain.m_dedr_min[j][0] = tree->m_cell_dedr_min[j][0];
				m_ikchain.m_dedr_min[j][1] = tree->m_cell_dedr_min[j][1];
				m_ikchain.m_dedr_min[j][2] = tree->m_cell_dedr_min[j][2];
			}
		}

		//m_ikchain.m_values = m_ikchain.m_average_values;
		//FLOG<<"start solving "<<std::endl;
		bool sol = true;
		//m_ikchain.m_dim_values = m_ikchain.m_average_values;
		if(i == 0)
		{
			/*for(int j = 0; j < m_ikchain.m_dims.size();++j)
			{
			m_ikchain.m_dim_values[j] = iniv.m_values[j];
			}*/
			sol = solver->solve(&m_ikchain, Etoile::Vector3_(point.x, point.y, point.z), false);
		}
		else
			sol = solver->solve(&m_ikchain,Etoile::Vector3_(point.x, point.y, point.z), true);
		if(sol == true)
		{
			solvable += 1; 
		}
		//FLOG<<"end solving"<<std::endl;

		for(int j = 0; j < m_ikchain.m_joints.size() - 1;++j)
		{
			Etoile::IKChain::Joint* jo =  m_ikchain.m_joints[j];
			BVH::Joint* jointbvh =  bvh.getJoint(jo->m_name);
			for(int h = 0; h < 3; ++h)
			{
				jointbvh->m_dims[h].m_value = m_ikchain.m_dim_values[ jo->m_dims[h].m_idx ] * 180.0/3.14159265;
			}
		}
		Frame frame = bvh.createFrame();
		fs.push_back(frame);
	}

	FLOG<<"SOLVABLE " <<solvable <<" / " <<points.size()<<std::endl;
	std::cout<<"SOLVABLE " <<solvable <<" / " <<points.size()<<std::endl;
	TimeWin32 start2;
	double time2 = start2.getCurrentTime();
	float tdiff = start2.DiffTime(time1);
	std::cout<<"solveTrajectory timediff1 "<<tdiff<<std::endl;

	bvh.m_frames = fs;
	std::stringstream s;
	s<<_name+"_depth_"<<depth<<"_"<<"BVH.bvh";
	bvh.saveToBVHFile(s.str());
	bvh.m_frames = temp;
}

void OctreeSkeleton::solveOnePoint(const Vec3& point, int depth)
{
	TimeWin32 start;
	double time1 = start.getCurrentTime();
	depth = 10;
	Octree* tree = m_treeowner.p_octreeRoot->getSubTreeWithPointAndDepth(point, depth);
	for(int i = 0; i < m_ikchain.m_dims.size();++i)
	{
		m_ikchain.m_dim_anglelimites[i] = Etoile::Vector2_(tree->m_cell_min[i], tree->m_cell_max[i]);
		//m_ikchain.m_localRotations[i] = Etoile::AngleAxis_(tree->m_cell_min[i], m_ikchain.m_axis[i]);
	}

	/*FrameData d = m_framesData[tree->dataIndx[0]];
	std::cout<<d.points[0]<<" "<<d.points[1]<<" "<<d.points[2]<<std::endl;
	m_ikchain.update();
	int end = m_ikchain.m_globalPositions.size() - 1;
	std::cout<< m_ikchain.m_globalPositions.back().transpose()<<std::endl;*/

	TimeWin32 start2;
	double time2 = start2.getCurrentTime();
	float tdiff = start2.DiffTime(time1);
	//std::cout<<"timediff1 "<<tdiff<<std::endl;

	TimeWin32 start3;
	double time3 = start3.getCurrentTime();
	solver->solve(&m_ikchain, Etoile::Vector3_(point.x, point.y, point.z));

	TimeWin32 start4;
	double time4 = start4.getCurrentTime();
	float tdiff2 = start4.DiffTime(time3);
	std::cout<<"timediff2 "<<tdiff2<<std::endl;

}



void OctreeSkeleton::computePoints()
{
	xMin = 10000, yMin = 10000, zMin = 10000;
	xMax = -10000, yMax = -10000, zMax = -10000;
	std::cout << " start compute points!"<<std::endl;
	for(unsigned int j = 0; j < m_framesData.size(); ++j)
	{
		for(int i = 0 ; i < m_ikchain.m_dims.size(); ++i)
		{
			m_ikchain.m_dim_values[i] = m_framesData[j].m_values[i];
		}
		m_ikchain.updateAllDims();
		m_ikchain.update();
		int end = m_ikchain.m_dim_globalPositions.size() - 1;

		//std::cout<< m_ikchain.m_globalPositions.back().transpose()<<std::endl;


		m_framesData[j].points[0] = m_ikchain.m_dim_globalPositions[end][0];
		m_framesData[j].points[1] = m_ikchain.m_dim_globalPositions[end][1];
		m_framesData[j].points[2] = m_ikchain.m_dim_globalPositions[end][2];
		if(m_framesData[j].points[0] > xMax) xMax = m_framesData[j].points[0];
		if(m_framesData[j].points[0] < xMin) xMin = m_framesData[j].points[0];
		if(m_framesData[j].points[1] > yMax) yMax = m_framesData[j].points[1];
		if(m_framesData[j].points[1] < yMin) yMin = m_framesData[j].points[1];
		if(m_framesData[j].points[2] > zMax) zMax = m_framesData[j].points[2];
		if(m_framesData[j].points[2] < zMin) zMin = m_framesData[j].points[2];

		//std::cout<< m_framesData[j].points[0] << " " << m_framesData[j].points[1] <<" "  << m_framesData[j].points[2]   <<std::endl;

		//std::cout<< j<<" "<<m_ikchain.m_globalPositions[end].transpose() <<std::endl;
		if(j % 100 == 0)
		{
			std::cout<<".";
		}
	}
	std::cout << "  end compute points!"<<std::endl;
	m_treeowner.p_octreeRoot = new Octree(Vec3((xMax + xMin) * 0.5, (yMax + yMin) * 0.5, (zMax + zMin) * 0.5), Vec3((xMax - xMin) * 0.5 + 0.1, (yMax - yMin) * 0.5 + 0.1, (zMax - zMin) * 0.5 + 0.1), &m_treeowner, -1);
}

void OctreeSkeleton::loadFromCSVFile(const std::string& filename)
{
	m_headers.clear();
	m_framesData.clear();
	std::fstream in(filename.c_str(), std::ios_base::in );

	if (!in.is_open() || !in.good())
	{
		std::cerr << "[OctreeSkeleton] : cannot not open file "
			<< filename
			<< std::endl;
	}
	std::cout << " read CSV file starts!"<<std::endl;
	int lineidx = 0;
	std::string line;
	while( in && !in.eof() )
	{
		std::getline(in,line);
		if ( in.bad() ){
			std::cout << "  Warning! Could not read file properly!"<<std::endl;
		}
		else
		{
			if(lineidx == 0)
			{
				if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
					continue;
				}
				m_headers.clear();
				std::stringstream stream(line);
				std::string valueString;
				while (std::getline(stream, valueString, ','))
				{
					trimString(valueString);
					if(!valueString.empty())
						m_headers.push_back(valueString);
				}
			}
			else
			{
				if ( line.size() == 0 || line[0] == '#' || isspace(line[0]) || line.empty() ) {
					continue;
				}
				int indx = m_framesData.size();
				m_framesData.push_back(FrameData());

				std::stringstream stream(line);
				std::string valueString;
				while (std::getline(stream, valueString, ','))
				{
					trimString(valueString);
					if(!valueString.empty())
					{
						double value = std::stod(valueString);
						m_framesData[indx].m_values.push_back(value);
					}

				}
			}
		}
		++lineidx;

	}

	in.close();
	std::cout << " read CSV file finished!"<<std::endl;
	m_dataIsLoaded = true;
}


void OctreeSkeleton::saveCSVFileByFrameIdx(const std::string& filename, std::vector<int> indx)
{
	std::cout << " save " << filename <<" file starts!"<<std::endl;
	std::ofstream out;
	out.open(filename);

	for(unsigned int i = 0; i < m_headers.size(); ++i)
	{
		out<<m_headers[i]<<",";
	}
	out<<std::endl;
	for(unsigned int i = 0; i < indx.size(); ++i)
	{
		FrameData& frame = m_framesData[indx[i]];
		for(unsigned int j = 0; j < frame.m_values.size(); ++j)
		{
			out<<frame.m_values[j]<<",";
		}
		out<<std::endl;
	}
	out.close();
	std::cout << " save " << filename <<" file ended!"<<std::endl;
}


void OctreeSkeleton::loadDataIntoOctree()
{
	if(m_dataIsLoaded || m_framesData.size() < 1)
	{
		std::cout<<"data is not loaded this time"<<std::endl;
		return;
	}

	std::cout<<"loading data into Octree"<<std::endl;
	for(unsigned int i = 0; i < m_framesData.size(); ++i)
	{
		FrameData& frame = m_framesData[i];
		//right
		{
			OctreePoint* point = new OctreePoint();
			point->position = Vec3(frame.points[0],frame.points[1],frame.points[2]);
			point->m_frameIdx = i;
			m_treeowner.p_octreeRoot->insert(point);
		}

		if(i % 100 == 0)
		{
			std::cout<<".";
		}
	}

	std::cout<<"right hand data finished"<<std::endl;

	//int Nsize = m_framesData.size();
	//for(unsigned int i = 0; i < m_framesData.size(); ++i)
	//{
	//	FrameData& frame = m_framesData[i];
	//	//right
	//	{
	//		OctreePoint* point = new OctreePoint();
	//		point->position = Vec3(frame.m_values[m_leftHand_index], frame.m_values[m_leftHand_index + 1], frame.m_values[m_leftHand_index + 2]);
	//		point->m_frameIdx = i + Nsize;
	//		p_tree->insert(point);
	//	}

	//	if(i % 100 == 0)
	//	{
	//		std::cout<<".";
	//	}
	//}
	std::cout<<"left hand data finished"<<std::endl;

	std::cout<<std::endl<<"loading data into Octree is finished"<<std::endl;
}


void OctreeSkeleton::computeCellAtributes(Octree* cell)
{
	if(cell->m_dataIndex.size() > 0)
	{
		int vSize = m_framesData[0].m_values.size();
		cell->m_cell_average.resize(vSize);
		cell->m_cell_min.resize(vSize);
		cell->m_cell_max.resize(vSize);
		cell->m_cell_dedr_max.resize(vSize);
		cell->m_cell_dedr_min.resize(vSize);
		cell->m_lamda.resize(vSize);
		/*cell->m_drData_positive.resize(vSize);
		cell->m_drrhs_positive.resize(vSize);
		cell->m_drParameter_positive.resize(vSize);
		cell->m_drData_negative.resize(vSize);
		cell->m_drrhs_negative.resize(vSize);
		cell->m_drParameter_negative.resize(vSize);*/
		for(int j = 0; j < vSize; ++j)
		{
			cell->m_cell_min[j] = 100;
			cell->m_cell_max[j] = -100;
			cell->m_cell_dedr_max[j] = Vec3(-100,-100,-100);
			cell->m_cell_dedr_min[j] = Vec3(100,100,100);
		}
		int positive = 0;
		int negative = 0;
		int nb_lamda = 0;
		for(unsigned int j = 0; j < cell->m_dataIndex.size(); ++j)
		{
			int idex = cell->m_dataIndex[j];
			if(idex - 1 > 0)
			{
				std::vector<double> lamda(vSize);
				computeLamda(m_ikchain, m_framesData[idex - 1], m_framesData[idex], lamda);
				for(int i = 0; i < vSize; ++i)
				{
					cell->m_lamda[i] = lamda[i];
				}
				++nb_lamda;
			}
			
		}
		double minLamda = 1;
		cell->m_size_lamda = nb_lamda;
		for(int i = 0; i < vSize; ++i)
		{
			cell->m_lamda[i] /= nb_lamda;
		}
		for(int i = 0; i < vSize; ++i)
		{
			if(cell->m_lamda[i] < minLamda && abs(cell->m_lamda[i] - 0) > 0.00000000001 )
			{
				minLamda = cell->m_lamda[i];
			}
		}
		for(int i = 0; i < vSize; ++i)
		{
			if(cell->m_lamda[i] < minLamda ) cell->m_lamda[i] = minLamda;
		}


		for(unsigned int j = 0; j < cell->m_dataIndex.size(); ++j)
		{
			FrameData& data = m_framesData[cell->m_dataIndex[j]];
			for(int h = 0; h < vSize; ++h)
			{
				cell->m_cell_min[h] = min(cell->m_cell_min[h], data.m_values[h]);
				cell->m_cell_max[h] = max(cell->m_cell_max[h], data.m_values[h]);
				cell->m_cell_average[h] += data.m_values[h];
				if( j > 0)
				{
					if((cell->m_dataIndex[j] - cell->m_dataIndex[j - 1] == 1))
					{

						double dr = data.m_values[h] - m_framesData[cell->m_dataIndex[j - 1]].m_values[h];
						double dx = data.points[0] - m_framesData[cell->m_dataIndex[j - 1]].points[0];
						double dy = data.points[1] - m_framesData[cell->m_dataIndex[j - 1]].points[1];
						double dz = data.points[2] - m_framesData[cell->m_dataIndex[j - 1]].points[2];

#ifdef USINGMAX
						cell->m_cell_dedr_max[h][0] = max (cell->m_cell_dedr_max[h][0], dx/dr);
						cell->m_cell_dedr_max[h][1] = max (cell->m_cell_dedr_max[h][1], dy/dr);
						cell->m_cell_dedr_max[h][2] = max (cell->m_cell_dedr_max[h][2], dz/dr);
						cell->m_cell_dedr_min[h][0] = min (cell->m_cell_dedr_min[h][0], dx/dr);
						cell->m_cell_dedr_min[h][1] = min (cell->m_cell_dedr_min[h][1], dy/dr);
						cell->m_cell_dedr_min[h][2] = min (cell->m_cell_dedr_min[h][2], dz/dr);
#endif
#ifdef USINGAV
						if(dr > 0)
						{
							cell->m_cell_dedr_max[h] = cell->m_cell_dedr_max[h] + dr;
							++positive;
						}
						if(dr < 0)
						{
							cell->m_cell_dedr_min[h] = cell->m_cell_dedr_min[h] + dr;
							++negative;
						}
#endif
#ifdef USINGLINEAREQUATIONè
						if(dr > 0)
						{
							cell->m_drData_positive[h].push_back(Etoile::Vector4_(dx,dy,dz,1));
							cell->m_drrhs_positive[h].push_back(dr);
						}
						if(dr < 0)
						{
							cell->m_drData_negative[h].push_back(Etoile::Vector4_(dx,dy,dz,1));
							cell->m_drrhs_negative[h].push_back(dr);
						}
#endif
					}
				}
			}
		}

		for(int j = 0; j < vSize; ++j)
		{
			cell->m_cell_average[j] /= cell->m_dataIndex.size();
#ifdef USINGAV
			cell->m_cell_dedr_max[j] /= positive;
			cell->m_cell_dedr_min[j] /= negative;

#endif
#ifdef USINGLINEAREQUATION
			if(cell->m_drData_positive[j].size() > 1)
			{
				Etoile::MatrixX_ x(cell->m_drData_positive[j].size(), 4) ;
				for (int h = 0; h < cell->m_drData_positive[j].size(); h++)
					for (int w = 0; w < 4; w++)
					{
						x(h, w) = cell->m_drData_positive[j][h][w];
					}
					Etoile::VectorX_ rhs(cell->m_drrhs_positive[j].size());
					for(int h = 0; h < cell->m_drrhs_positive[j].size(); ++h)
					{
						rhs[h] = cell->m_drrhs_positive[j][h];
					}
					Eigen::JacobiSVD<Etoile::MatrixX_> svd(x, Eigen::ComputeThinU | Eigen::ComputeThinV);
					cell->m_drParameter_positive[j] = svd.solve(rhs);
			}

			if(cell->m_drData_negative[j].size() > 1)
			{
				Etoile::MatrixX_ x(cell->m_drData_negative[j].size(), 4) ;
				for (int h = 0; h < cell->m_drData_negative[j].size(); h++)
					for (int w = 0; w < 4; w++)
					{
						x(h, w) = cell->m_drData_negative[j][h][w];
					}
					Etoile::VectorX_ rhs(cell->m_drrhs_negative[j].size());
					for(int h = 0; h < cell->m_drrhs_negative[j].size(); ++h)
					{
						rhs[h] = cell->m_drrhs_negative[j][h];
					}
					Eigen::JacobiSVD<Etoile::MatrixX_> svd(x, Eigen::ComputeThinU | Eigen::ComputeThinV);
					cell->m_drParameter_negative[j] = svd.solve(rhs);
			}

#endif
			//std::cout<<cell->m_drParameter[j].transpose()<<std::endl;
		}
		//debugValue(cell, out);
		////evaluation negative

		//	for(unsigned int j = 0; j < cell->dataIndx.size(); ++j)
		//	{
		//		FrameData& data = m_framesData[cell->dataIndx[j]];
		//		for(int h = 0; h < vSize; ++h)
		//		{
		//			if( j > 0)
		//			{
		//				if((cell->dataIndx[j] - cell->dataIndx[j - 1] == 1))
		//				{
		//					double dr = data.m_values[h] - m_framesData[cell->dataIndx[j - 1]].m_values[h];
		//					double dx = data.points[0] - m_framesData[cell->dataIndx[j - 1]].points[0];
		//					double dy = data.points[1] - m_framesData[cell->dataIndx[j - 1]].points[1];
		//					double dz = data.points[2] - m_framesData[cell->dataIndx[j - 1]].points[2];
		//					double dN = cell->m_drParameter_negative[h].dot(Etoile::Vector4_(dx,dy,dz,1));
		//					double dP = cell->m_drParameter_positive[h].dot(Etoile::Vector4_(dx,dy,dz,1));
		//					if(dN > 0)
		//						std::cout<<dN <<std::endl;
		//					if(dP < 0)
		//						std::cout<<dP <<std::endl;

		//				}
		//			}
		//		}
		//	}

	}

}

void OctreeSkeleton::computeMinMaxAverage()
{
	std::cout<<std::endl<<"compute Cell attributes starts"<<std::endl;
	std::ofstream out;
	out.open("cell.txt");
	int vSize = m_framesData[0].m_values.size();
	std::vector<Octree*>& octrees = m_treeowner.m_alltree;
	for(unsigned int i = 0; i < octrees.size(); ++i)
	{
		Octree* cell = octrees[i];
		computeCellAtributes(cell);
	}
	out.close();
	std::cout<<std::endl<<"compute Cell attributes ended"<<std::endl;
}

void OctreeSkeleton::computeLamda(Etoile::IKChain& chain, FrameData& current, FrameData& next, std::vector<double>& out_lamda)
{
	using namespace Etoile;
	chain.m_dim_values = current.m_values;
	chain.updateAllDims();
	MatrixX_ jacobian(3, chain.m_dims.size());
	Vector3_& endpos = chain.m_dim_globalPositions.back();
	Vector3_ target(next.points);
	Vector3_ distance = (target-endpos);

	VectorX_ dTheta(chain.m_dims.size());

	for(unsigned int j = 0; j < chain.m_dims.size(); ++j)
	{
		dTheta(j) = next.m_values[j] - current.m_values[j];

		IKChain::Dim* dim = chain.m_dims[j];
		Vector3_& jointPos = chain.m_dim_globalPositions[dim->m_idx];
		Vector3_ boneVector = endpos - jointPos;

		Vector3_ axis = chain.m_dim_axis[dim->m_idx];
		int lastDim = dim->m_lastIdx;
		if(lastDim >= 0)
		{
			axis = chain.m_dim_globalOrientations[lastDim] * axis;
		}
		Vector3_ axisXYZgradient = axis.cross(boneVector);
		jacobian(0, j) = /*clamp(0 == axisXYZgradient(0)? 0.000001:*/ axisXYZgradient(0)/*, chain->m_dedr_min[j][0], chain->m_dedr_max[j][0])*/;// * m_stepweight;
		jacobian(1, j) =/* clamp(0 == axisXYZgradient(1)? 0.000001:*/ axisXYZgradient(1)/*, chain->m_dedr_min[j][1], chain->m_dedr_max[j][1])*/;// * m_stepweight;
		jacobian(2, j) =/* clamp(0 == axisXYZgradient(2)? 0.000001:*/ axisXYZgradient(2)/*, chain->m_dedr_min[j][2], chain->m_dedr_max[j][2])*/;// * m_stepweight;
	}
	MatrixX_ jacobianTranspose = jacobian.transpose();
	VectorX_ rightside = jacobianTranspose * (distance - jacobian * dTheta);//jacobianTranspose * distance - jacobianTranspose * jacobian * dTheta;
	for(unsigned int j = 0; j < chain.m_dims.size(); ++j)
	{
		dTheta(j) = abs(dTheta(j)) > 0 ? dTheta(j) : 0.00000001;
		double v = abs(rightside(j) / dTheta(j));
		out_lamda[j] =  v > 0 ? v : 0.00000001;
		assert(out_lamda[j] > 0.0000000001);
		assert(out_lamda[j] < 99999999999999);
		//if(out_lamda[j] < 0) out_lamda[j] = 0;
	}
}


void OctreeSkeleton::computeMinMaxAverageByDepth(int depth)
{
	std::cout<<std::endl<<"compute Cell attributes starts"<<std::endl;
	std::ofstream out;
	out.open("cell.txt");
	int vSize = m_framesData[0].m_values.size();
	//std::vector<Octree*>& octrees = m_treeowner.m_alltree;
	//Octree* cell = p_tree;
	//computeCellAtributes(cell);
	std::queue<Octree*> cells;
	cells.push(m_treeowner.p_octreeRoot);
	while(!cells.empty())
	{
		Octree* cell = cells.front();
		computeCellAtributes(cell);

		if(cell->children[0] != NULL)
		{
			for(unsigned int i = 0; i < 8; ++i)
			{
				Octree* cell0 = cell->children[i];
				if(! (cell0->m_level > depth))
				{
					cells.push(cell0);
				}
			}
		}
		cells.pop();
	}
	out.close();
	std::cout<<std::endl<<"compute Cell attributes ended"<<std::endl;
}


float scaleInOne(float scale)
{
	return (scale + 3.14159265) / 3.14159265 * 0.5;
}

void OctreeSkeleton::draw()
{
	if(m_treeowner.m_alltree.size() == 0 || !_visible) return;
	glDisable(GL_LIGHTING);


	if(drawType == 0)
	{
		//std::vector<Octree*>& octrees = m_treeowner.m_alltree;
		std::queue<Octree*> cells;
		cells.push(m_treeowner.p_octreeRoot);
		while(!cells.empty())
		{
			Octree* cell = cells.front();
			if(cell->m_dataIndex.size() > 0)
			{
				//glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec3(0.8, 0.7, 1)[0]);
				glColor3f(0.8, 0.7, 1);
				cell->drawAABB();
			}

			if(cell->children[0] != NULL)
			{
				for(unsigned int i = 0; i < 8; ++i)
				{
					Octree* cell0 = cell->children[i];
					if(! (cell0->m_level > m_depth))
					{
						cells.push(cell0);
					}
				}
			}
			cells.pop();
		}

		glPointSize(2);
		glBegin(GL_POINTS);
		for(unsigned int i = 0; i < m_framesData.size(); ++i)
		{
			FrameData& frame = m_framesData[i];
			//right
			{
				//glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec3(1, 0.0,0)[0]);
				glColor3f(1, 0.0,0);
				glVertex3f(frame.points[0],frame.points[1],frame.points[2]);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	else if(drawType == 1)
	{
		std::queue<Octree*> cells;
		cells.push(m_treeowner.p_octreeRoot);
		while(!cells.empty())
		{
			Octree* cell = cells.front();
			if(cell->m_dataIndex.size() > 0)
			{
				//glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec3(0.8, 0.7, 1)[0]);
				glColor3f(0.8, 0.7, 1);
				cell->drawAABB();
				if(cell->m_level == m_depth)
				{
					glColor4f(scaleInOne(cell->m_cell_min[15]), scaleInOne(cell->m_cell_min[16]), scaleInOne(cell->m_cell_min[17]), 1);
					cell->drawAABBBox();
				}
			}

			if(cell->children[0] != NULL)
			{
				for(unsigned int i = 0; i < 8; ++i)
				{
					Octree* cell0 = cell->children[i];
					if(! (cell0->m_level > m_depth))
					{
						cells.push(cell0);
					}
				}
			}
			cells.pop();
		}
	}
	else if(drawType == 2)
	{
		std::queue<Octree*> cells;
		cells.push(m_treeowner.p_octreeRoot);
		while(!cells.empty())
		{
			Octree* cell = cells.front();
			if(cell->m_dataIndex.size() > 0)
			{
				//glMaterialfv(GL_FRONT, GL_DIFFUSE, &Vec3(0.8, 0.7, 1)[0]);
				glColor3f(0.8, 0.7, 1);
				cell->drawAABB();
				if(cell->m_level == m_depth)
				{
					glColor4f(scaleInOne(cell->m_cell_max[15]), scaleInOne(cell->m_cell_max[16]), scaleInOne(cell->m_cell_max[17]), 1);
					cell->drawAABBBox();
				}
			}

			if(cell->children[0] != NULL)
			{
				for(unsigned int i = 0; i < 8; ++i)
				{
					Octree* cell0 = cell->children[i];
					if(! (cell0->m_level > m_depth))
					{
						cells.push(cell0);
					}
				}
			}
			cells.pop();
		}
	}else if(drawType == 3)
	{
		for(int i = 1 ; i < m_ikchain.m_joints.size();++i)
		{
			int id = i;
			int parent = m_ikchain.m_joints[i]->m_index_parent;
			Etoile::Vector3_& pos1 = m_ikchain.m_joint_globalPositions[id];
			Etoile::Vector3_& pos2 = m_ikchain.m_joint_globalPositions[parent];

			glLineWidth(1.0f); 
			glBegin(GL_LINES);
			glVertex3f(pos1(0),pos1(1),pos1(2));
			glVertex3f(pos2(0),pos2(1),pos2(2));
			glEnd();
		}
	}

}