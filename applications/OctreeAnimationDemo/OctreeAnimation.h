#pragma once
#include "Octree.h"
#include <string>
#include <map>
#include "IKChain.h"
#include "BVH.h"
#include "JacobianDLSSolver.h"

struct Sequence
{
	std::string m_filename;
	std::vector<std::vector<Vector3_>> m_values_frames;
	std::vector<std::string> m_headers;
	std::map<std::string, int> m_indexes;
};



class OctreeAnimation
{
	Octree m_octree;
	std::map<std::string, Sequence*> m_usedSequences;
	Etoile::IKChain m_chain;
	Etoile::JacobianDLSSolver* solver;
	BVH* _originalBVH;
public:
	OctreeAnimation(void);
	~OctreeAnimation(void);

	void addBVH(const std::string& filename);
	void addBVH(BVH*);
	void addSequence(Sequence*);
	void updateOctree();
	void saveOctree(const std::string& filename);
	void loadOctree(const std::string& filename);
	void saveMyOctree(const std::string& filename);


	void saveTXT(const std::string& filename){
		std::cout<<"start writing! "<<std::endl;
        std::ofstream ofs(filename);
        boost::archive::text_oarchive oa(ofs);
        oa << m_octree;
        ofs.close();
		std::cout<<"end writing! "<<std::endl;
    }

	void loadTXT(const std::string& filename){
		std::cout<<"start reading! "<<std::endl;
		m_octree = Octree();
        std::ifstream ifs(filename);
        boost::archive::text_iarchive ia(ifs);
        ia >> m_octree;
        ifs.close();
		std::cout<<"end reading! "<<std::endl;
    }

	void solve(const std::vector<Vector3_>& points, BVH& bvh);
	void solveOriginalTrajectory(int start, int end, const std::string& bvhfile = "");
	void solve(const std::vector<Vector3_>& points);
};

