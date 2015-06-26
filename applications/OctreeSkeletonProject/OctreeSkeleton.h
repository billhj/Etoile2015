#pragma once
#include <string>
#include <vector>
#include "Octree.h"

struct FrameData
{
	std::vector<double> m_values;
};

class OctreeSkeleton
{
public:
	OctreeSkeleton(void);
	~OctreeSkeleton(void);
	void loadFromCSVFile(const std::string& filename);
	void saveCSVFileByFrameIdx(const std::string& filename, std::vector<int> indx);

	void loadDataIntoOctree();

	void computeMinMaxAverage();

	void computeMinMaxAverageByDepth(int level);

	void computeCellAtributes(Octree* tree);

	std::vector<std::string> m_headers;
	std::vector<FrameData> m_framesData;

	int m_leftHand_index;
	int m_rightHand_index;

	Octree * p_righthandTree;
	bool m_dataIsLoaded;
};

