#include "OctreeSkeleton.h"

int main(int argc, char *argv[])
{
	OctreeSkeleton sk;
	/*for(int i = 0 ; i < 200; ++i)
	{
		double* p = sk.m_framesData[i].points;
		sk.solveOnePoint(Vec3(p[0],p[1],p[2]));
	}*/
	
	std::vector<Vec3> points;
	for(int i = 620 ; i < 650; ++i)
	{
		double* p = sk.m_framesData[i].points;
		points.push_back(Vec3(p[0],p[1],p[2]));
	}
	sk.solveTrajectory(points);
	/*{
		Octree* sub = sk.p_righthandTree->getSubTreeWithPointAndDepth(Vec3(-0.2, 0.4, 0.4), 30);
		sk.saveCSVFileByFrameIdx("1.csv",sub->dataIndx);
	}

	{
		Octree* sub = sk.p_righthandTree->getSubTreeWithPointAndDepth(Vec3(-0.2, 0.3, 0.3), 30);
		sk.saveCSVFileByFrameIdx("2.csv",sub->dataIndx);
	}

	{
		Octree* sub = sk.p_righthandTree->getSubTreeWithPointAndDepth(Vec3(-0.3, 0.3, 0.2), 30);
		sk.saveCSVFileByFrameIdx("3.csv",sub->dataIndx);
	}

	{
		Octree* sub = sk.p_righthandTree->getSubTreeWithPointAndDepth(Vec3(-0.3, 0.2, 0.2), 30);
		sk.saveCSVFileByFrameIdx("4.csv",sub->dataIndx);
	}

	{
		Octree* sub = sk.p_righthandTree->getSubTreeWithPointAndDepth(Vec3(-0.2, 0.1, 0.1), 30);
		sk.saveCSVFileByFrameIdx("5.csv",sub->dataIndx);
	}*/

	return 0;
}
