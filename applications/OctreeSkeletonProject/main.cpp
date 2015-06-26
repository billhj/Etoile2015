#include "OctreeSkeleton.h"

int main(int argc, char *argv[])
{
	OctreeSkeleton sk;
	sk.loadFromCSVFile("pts_anger_brian.csv");
	sk.loadDataIntoOctree();
	sk.computeMinMaxAverageByDepth(3);
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
