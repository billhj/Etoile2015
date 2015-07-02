#pragma once
#include <string>
#include <vector>
#include "Octree.h"
#include "BVH.h"
#include "IKChain.h"
#include "JacobianDLSSVDSolver.h"

struct FrameData
{
	std::vector<double> m_values;
	double points[3];
};

class OctreeSkeleton
{
public:
	OctreeSkeleton(void);
	~OctreeSkeleton(void);

	void solveTrajectory(const std::vector<Vec3>& points, int depth = 2);
	void solveOnePoint(const Vec3& point, int depth = 2);



	void loadFromCSVFile(const std::string& filename);
	void computePoints();
	void saveCSVFileByFrameIdx(const std::string& filename, std::vector<int> indx);
	void loadDataIntoOctree();
	void computeMinMaxAverage();
	void computeMinMaxAverageByDepth(int level);
	void computeCellAtributes(Octree* tree);

	std::vector<std::string> m_headers;
	std::vector<FrameData> m_framesData;

	int m_rightHand_index;

	Octree * p_tree;  //i change tree for both hand since two hands are simetric.
	bool m_dataIsLoaded;

	BVH m_bvh;
	Etoile::IKChain m_ikchain;
	Etoile::JacobianDLSSVDSolver * solver;

};

