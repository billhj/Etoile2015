#pragma once
#include <string>
#include <vector>
#include "Octree.h"
#include "animation/BVH.h"
#include "animation/IK/IKchain.h"
#include "animation/IK/JacobianDLSSVDSolver.h"
#include "animation/IK/JacobianDLSSolver.h"
#include "animation/IK/JacobianRcompensedDLSSolver.h"
#include "geometry/ObjectRenderer.h"

struct FrameData
{
	std::vector<double> m_values;
	double points[3];
};

class OctreeSkeleton : public Etoile::ObjectRenderer
{
public:
	OctreeSkeleton(void);
	//OctreeSkeleton(const std::string& name);
	~OctreeSkeleton(void);
	void reload(const std::string& name);
	void solveTrajectory(const std::vector<Vec3>& points, int depth, BVH&);
	void solvePrefilterTrajectory(const std::vector<Vec3>& points, int depth, BVH&);
	void solveOriginalTrajectory(int start, int end, const std::string& bvhfile = "");
	void solveOriginalPrefilterTrajectory(int start, int end, const std::string& bvhfile = "");

	void solveOnePoint(const Vec3& point, int depth = 2);



	void loadFromCSVFile(const std::string& filename);
	void computePoints();
	void saveCSVFileByFrameIdx(const std::string& filename, std::vector<int> indx);
	void loadDataIntoOctree();
	void computeMinMaxAverage();
	void computeMinMaxAverageByDepth(int level);
	void computeCellAtributes(Octree* tree);


	void computeLamda(Etoile::IKChain& chain, FrameData& current, FrameData& target, std::vector<double>& out_lamda);

	std::vector<std::string> m_headers;
	std::vector<FrameData> m_framesData;

	int m_rightHand_index;

	OctreeOwner m_treeowner;  //i change tree for both hand since two hands are simetric.
	bool m_dataIsLoaded;

	BVH m_bvh;
	Etoile::IKChain m_ikchain;
	Etoile::JacobianDLSSolver * solver;
	std::string _name;
	double xMin, yMin, zMin;
	double xMax, yMax, zMax;
	double _limit;

	int drawType;
	int m_depth;

	
	virtual void draw();
	virtual void drawTexcoord(){}
};

